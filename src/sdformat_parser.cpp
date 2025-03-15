/*
* sdformat-editor
* Copyright 2025 sdformat-editor
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Developer: Zaid Duraid, Ean Wheeler, Evan Vokey
*/

// NOTE: (zaid) Functions related to file I/O which are directly under the sdf namespace:
// https://osrf-distributions.s3.amazonaws.com/sdformat/api/14.0.0/namespacesdf_1_1SDF__VERSION__NAMESPACE.html#adfb477b41b7b8f4018b6ba90cc587995

#include "sdformat_parser.h"
#include <stack>

void SDFormatParser::Initialize(std::string file_path, bool &success)
{
  success = false;

  // This is a vector of objects of the `std::Error` class
  sdf::Errors errors; 

  // Create a new SDF element pointer
  this->sdfElement.reset(new sdf::SDF);

  // NOTE: (zaid)
  // The SDF spec based on 1.11 is embedded into the application (assuming we downloaded the version associated with 1.11)
  // it can simply be obtained by:
  // std::string spec = sdf::SDF::EmbeddedSpec("root.sdf", false)
  // This returns an XML string which can be parsed in the following way
  // auto xmlDoc = makeSdfdDoc(); xmlDoc.Parse(spec.c_str());
  // Where makeSdfDoc just returns tinyxml2::XMLDocument(true, tinyxml2::COLLAPSE_WHITESPACE) (see source code in https://github.com/gazebosim/sdformat/blob/71fe5048d533ea2f422fc6f0b0b98f75e22e0720/src/parser.cc#L140)
  
  // Everything above is done in the `sdf::init` function below, which takes in an SDFPtr. 
  if (!sdf::init(errors, sdfElement, sdf::ParserConfig::GlobalConfig())) // We use the overloaded version of `init` that allows for passing in the `errors` vector.
  {
    std::cerr << "Could not read SDF Spec to autofill the main root SDF element!" << std::endl;
    // TODO: (zaid) Do something with the `errors` vector
    return;
  }
  // NOTE: (zaid) The init function will do the following:
  // Call `initDoc` on the SDFPtr (and treat is as an SDFElement, see API in source code). 
  // This will start a large recursive process where we do the following:
  // - If this is an element with a `type` as per the spec (not the case for the root SDF file), then add it's default value, description, min, max, and whether or not it's required. This is done via `AddValue()`
  // - Loop through all possible attributes of this element and add the corresponding default parameters as per the spec
  // - Loop through all possible nested elements recusively and autofill them the same way described here
  // - It is possible to include a nested element indirectly in the spec by referencing the spec file by having `include` in the spec. In this case, use `EmbeddedSpec` to locate the spec and do the same thing recursively
  // After init, you end up with an entire SDF tree filled with default values.

  if (!sdf::readFile(file_path, sdfElement, errors))
  {
    std::cerr << file_path << " is not a valid SDF file!" << std::endl;
    // TODO: (zaid) Do something with the `errors` vector
    return;
  }

  // NOTE: (zaid) The readFile function will go through the given file. This includes the top level element, it's attributes, and recursively all the nested elements. 
  // Nested elements can also be included using `include` tages, where references to other files are made
  // In the future, it's possible to allow the user to define custom elements that are not specified by the spec.
  // All that needs to be done is store somewhere the configuration for these custom elements and corresponding parent elements.
  
  success = true;
}

sdf::SDFPtr SDFormatParser::GetSDFElement()
{
  return this->sdfElement;
}

SDFormatParser::Mentions SDFormatParser::FindMentions(std::string key)
{
  return this->FindMentions(key, nullptr, nullptr);
}

SDFormatParser::Mentions SDFormatParser::FindMentions(std::string key, sdf::ElementPtr element_to_exclude)
{
  // Exclude the element_to_exclude's obvious name attribute as well
  return this->FindMentions(key, element_to_exclude, element_to_exclude->GetAttribute("name"));
}

SDFormatParser::Mentions SDFormatParser::FindMentions(std::string key, sdf::ParamPtr attribute_to_exclude)
{
  return this->FindMentions(key, nullptr, attribute_to_exclude);
}

SDFormatParser::Mentions SDFormatParser::FindMentions(std::string key, sdf::ElementPtr element_to_exclude, sdf::ParamPtr attribute_to_exclude)
{
  
  // Define a Mentions object
  SDFormatParser::Mentions mentions;

  // Exit if we don't have a root sdf element
  if (!this->sdfElement) return mentions;

  // Create a stack to hold the element to search for
  // The boolean indicates whether or not this tree node was visited
  std::stack<std::pair<sdf::ElementPtr,bool>> sdf_tree_stack;

  // Append the root model element to the stack
  sdf_tree_stack.emplace(this->sdfElement->Root()->GetFirstElement(), false);

  while (!sdf_tree_stack.empty())
  {
    // If this node has already been visited and we are returning to it, 
    // it means we have accounted it and for all of it's child nodes.
    // In that case, pop the node from the stack
    if (sdf_tree_stack.top().second)
    {
      sdf_tree_stack.pop();
      continue;
    }

    sdf_tree_stack.top().second = true;

    // Get a pointer to the current node
    sdf::ElementPtr current_element = sdf_tree_stack.top().first;

    // Check for reference equality between the current element and the element to exclude
    if (!(current_element.get() == element_to_exclude.get()))
    {
      // Check if this element has an associated value (i.e. it is a type element)
      if (auto value = current_element->GetValue())
      {
        // Check if this element references the key
        if (value->GetTypeName() == "string" && value->GetAsString() == key)
        {
          mentions.elements.push_back(current_element);
        }
      }
    }
    
    // Go through each attribute of this element...
    for (const auto &current_attribute : current_element->GetAttributes())
    {
      // Check for reference equality between the current attribute and the attribute to exclude
      if (!(current_attribute.get() == attribute_to_exclude.get()))
      {
        // Check if this attribute references the key
        if (current_attribute->GetTypeName() == "string" && current_attribute->GetAsString() == key)
        {
          mentions.attributes.push_back(current_attribute);
        }
      }
    }

    if (current_element->GetFirstElement())
    {
      // Go through each element 
      sdf::ElementPtr child_element_ptr = current_element->GetFirstElement();
      while (child_element_ptr)
      {
        // Add this child element to the stack
        sdf_tree_stack.emplace(child_element_ptr, false);
        child_element_ptr = child_element_ptr->GetNextElement("");
      }
    } else
    {
      // If this element has no children (leaf node), then it may be removed from the stack
      sdf_tree_stack.pop();
      continue;
    }
  }

  return mentions;
}

std::string SDFormatParser::GetSDFTreePathToElement(sdf::ElementPtr current_element)
{

  std::string sdf_tree_path_to_element;

  while (current_element)
  {
    std::string name;

    // The name of the element is what the "name" attribute is set to.
    // However, if there is no name attribute, then the name of the 
    // element is considered to be the element type (i.e. return value of GetName())
    if (current_element->GetAttribute("name"))
    {
      name = current_element->GetAttribute("name")->GetAsString();
    }
    else
    {
      name = current_element->GetName();
    }

    if (!sdf_tree_path_to_element.empty())
    {
      sdf_tree_path_to_element = name + "::" + sdf_tree_path_to_element;
    }
    else
    {
      sdf_tree_path_to_element = name;
    }

    current_element = current_element->GetParent();
  }

  return sdf_tree_path_to_element;
} 