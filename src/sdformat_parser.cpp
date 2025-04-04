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
  
  this->sdf_file_path = file_path;

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
  SDFormatParser::Mentions mentions;

  if (!this->sdfElement) return mentions;

  // Create a stack to hold the element to search for
  // The boolean indicates whether or not this tree node was visited
  std::stack<std::pair<sdf::ElementPtr,bool>> sdf_tree_stack;

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

    sdf::ElementPtr current_element = sdf_tree_stack.top().first;

    // Check for reference equality between the current element and the element to exclude
    if (!(current_element.get() == element_to_exclude.get()))
    {
      if (auto value = current_element->GetValue())
      {
        if (value->GetTypeName() == "string" && value->GetAsString() == key)
        {
          mentions.elements.push_back(current_element);
        }
      }
    }
    
    for (const auto &current_attribute : current_element->GetAttributes())
    {
      if (!(current_attribute.get() == attribute_to_exclude.get()))
      {
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

std::vector<sdf::ElementPtr> SDFormatParser::LookupElementsByAttributeType(const std::string& attribute_type)
{
  return this->LookupElementsInternal(attribute_type, "", this->sdfElement->Root(), nullptr);
}

std::vector<sdf::ElementPtr> SDFormatParser::LookupElementsByAttributeTypeAndValue(const std::string& attribute_type, const std::string& attribute_value)
{
  return this->LookupElementsInternal(attribute_type, attribute_value, this->sdfElement->Root(), nullptr);
}


std::vector<sdf::ElementPtr> SDFormatParser::LookupElementsInternal(const std::string& attribute_type, const std::string& attribute_value,
                                                                                      sdf::ElementPtr scope, sdf::ElementPtr scope_to_exclude)
{
  std::vector<sdf::ElementPtr> elements;

  if (!scope) return elements;

  // Create a stack to hold the element to search for
  // The boolean indicates whether or not this tree node was visited
  std::stack<std::pair<sdf::ElementPtr,bool>> sdf_tree_stack;

  sdf_tree_stack.emplace(scope, false);

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

    sdf::ElementPtr current_element = sdf_tree_stack.top().first;

    // If there is a scope that we are excluding out of the search, 
    // we can consider the root element of this scope to be a leaf node
    if (scope_to_exclude)
    {
      if (current_element.get() == scope_to_exclude.get())
      {
        sdf_tree_stack.pop();
        continue;
      }
    }

    if (current_element->HasAttribute(attribute_type)) 
    {
      sdf::ParamPtr attribute = current_element->GetAttribute(attribute_type);
      if (attribute_value != "")
      {
        if (attribute->GetAsString() == attribute_value) elements.push_back(current_element);
      }
      else
      {
        elements.push_back(current_element);
      }
    }
    
    if (current_element->GetFirstElement())
    {
      sdf::ElementPtr child_element_ptr = current_element->GetFirstElement();
      while (child_element_ptr)
      {
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

  // If we did not reach the root element in terms of scope, continue checking outer scopes
  if (scope.get() != this->sdfElement->Root().get()) 
  {
    // Exclude the current scope as we search outer scopes so we don't add the same elements again
    auto outer_scope_elements = this->LookupElementsInternal(attribute_type, attribute_value, this->FindScope(scope), scope);
    elements.insert(elements.end(), outer_scope_elements.begin(), outer_scope_elements.end());
  }
  return elements;
}

std::vector<sdf::ElementPtr> SDFormatParser::LookupElementsByType(const std::string& type)
{
  std::vector<sdf::ElementPtr> elements;

  if (!this->sdfElement) return elements;

  // Create a stack to hold the element to search for
  // The boolean indicates whether or not this tree node was visited
  std::stack<std::pair<sdf::ElementPtr,bool>> sdf_tree_stack;

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

    sdf::ElementPtr current_element = sdf_tree_stack.top().first;

    if (current_element->GetName() == type) elements.push_back(current_element);
    
    if (current_element->GetFirstElement())
    {
      sdf::ElementPtr child_element_ptr = current_element->GetFirstElement();
      while (child_element_ptr)
      {
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

  return elements;

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

sdf::ElementPtr SDFormatParser::FindScope(sdf::ElementPtr element)
{
  while (element->GetParent())
  {
    if (element->GetParent()->GetName() == "model" || element->GetParent()->GetName() == "world") 
    {
      return element->GetParent();
    }
    element = element->GetParent();
  }

  return this->sdfElement->Root();
}

sdf::ElementPtr SDFormatParser::FindCanonical(sdf::ElementPtr model_element)
{
  if (!(model_element->GetName() == "model")) return nullptr;

  if (model_element->HasAttribute("canonical_link") && model_element->GetAttribute("canonical_link")->GetAsString() != "")
  {
    if (model_element->HasElement(model_element->GetAttribute("canonical_link")->GetAsString()))
    {
      return model_element->GetElement(model_element->GetAttribute("canonical_link")->GetAsString());
    }
  }

  // If we are here, no explicit canonical link was found

  // Iterate through in an attempt to find the first child link
  sdf::ElementPtr child_element = model_element->GetFirstElement();
  while (child_element)
  {
    if (child_element->GetName() == "link")
    {
      return child_element;
    }
    child_element = child_element->GetNextElement();
  }

  // If we are here, there is no child link elements at all

  // Iterate through in an attempt to find the first child model
  child_element = model_element->GetFirstElement();
  while (child_element)
  {
    if (child_element->GetName() == "model")
    {
      return child_element;
    }
    child_element = child_element->GetNextElement();
  }

  std::cerr << this->GetSDFTreePathToElement(model_element) << " does not have a canonical link or nested models." << std::endl; 

  return nullptr;
}

bool SDFormatParser::HandleRelativeToSpecificationSpecialcases(sdf::ElementPtr element, std::string& relative_to)
{
  if (relative_to == "__model__")
  {
    while (element->GetParent())
    {
      if (element->GetParent()->GetName() == "model") 
      {
        if (element->GetParent()->HasAttribute("name"))
        {
          relative_to = element->GetParent()->GetAttribute("name")->GetAsString();
        }
        break;
      }
      element = element->GetParent();
    }
    if (relative_to == "__model__")
    {
      std::cerr << "Could not find a valid model for_relative specification of " 
                  << this->GetSDFTreePathToElement(element) << std::endl;
      return false;
    }
  }
  else if (relative_to == "world")
  {
    while (element->GetParent())
    {
      if (element->GetParent()->GetName() == "world") 
      {
        if (element->GetParent()->HasAttribute("name"))
        {
          relative_to = element->GetParent()->GetAttribute("name")->GetAsString();
        }
        break;
      }
      element = element->GetParent();
    }
    if (relative_to == "world")
    {
      std::cerr << "Could not find a valid world for_relative specification of " 
                  << this->GetSDFTreePathToElement(element) << std::endl;
      return false;
    }
  }
  return true;
}

std::pair<glm::dvec3, glm::dquat> SDFormatParser::FindAbsolutePose(sdf::ElementPtr element, std::vector<sdf::ElementPtr> previously_visited_elements)
{
  std::pair<glm::dvec3, glm::dquat> pose = std::make_pair(glm::dvec3(0.0f), glm::dquat());
  pose.second = glm::dquat(1.0, 0.0, 0.0, 0.0); // Identity quaternion

  std::string relative_to = "";

  // Attempt to get the pose if it is defined
  if (element->GetFirstElement())
  {
    sdf::ElementPtr child_element_ptr = element->GetFirstElement();
    while (child_element_ptr)
    {
      if (child_element_ptr->GetName() == "pose")
      { 
        pose = this->ParsePoseElement(child_element_ptr, relative_to);
        break;
      }
      child_element_ptr = child_element_ptr->GetNextElement("");
    }
  } 

  if (relative_to != "")
  {
    if (!HandleRelativeToSpecificationSpecialcases(element, relative_to))
    {
      return pose;
    }
  }
  else 
  {
    if (element->GetName() == "frame")
    {
      // If we have a frame with no non-empty "relative_to" attribute, we use the "attached_to" attribute instead
      if (element->HasAttribute("attached_to") && element->GetAttribute("attached_to")->GetAsString() != "")
      {
        relative_to == element->GetAttribute("attached_to")->GetAsString();
        if (!HandleRelativeToSpecificationSpecialcases(element, relative_to))
        {
          return pose;
        }
      }
    }
    else if (element->GetName() == "model")
    {
      if (sdf::ElementPtr canonical_element = this->FindCanonical(element))
      {
        if (canonical_element->HasAttribute("name") && canonical_element->GetAttribute("name")->GetAsString() != "")
        {
          relative_to = canonical_element->GetAttribute("name")->GetAsString();
        }
        else
        {
          std::cerr << "Could not find a valid canonical link or nested model element for " 
          << this->GetSDFTreePathToElement(element) << std::endl;
          return pose;
        }
      }
    }
    else if (element->GetName() == "joint")
    {
      bool has_child = false;
      sdf::ElementPtr child_element_ptr = element->GetFirstElement();
      while (child_element_ptr)
      {
        if (child_element_ptr->GetName() == "child")
        {
          has_child = true;

          // Joints without a relative_to attribute will have their position be defined relative to their child element
          // We need to locate the element referenced by the child and ensure that it is a link

          if (child_element_ptr->GetValue()->GetAsString() == "" || child_element_ptr->GetValue()->GetAsString() == "world")
          {
            std::cerr << "Invalid child specified for  " 
                        << this->GetSDFTreePathToElement(element) << std::endl;
            return pose;
          }
          
          std::vector<sdf::ElementPtr> possibly_referenced_child_elements = this->LookupElementsInternal("name", child_element_ptr->GetValue()->GetAsString(),
                                                                                    this->FindScope(child_element_ptr), nullptr);
          
          if (possibly_referenced_child_elements.size() <= 0)
          {
            std::cerr << "Could not find elements referenced by the child of  " 
                        << this->GetSDFTreePathToElement(element) << std::endl;
            return pose;
          }
          else
          {
            // LookupElementsInternal will return the referenced elements by order of when they were found in scopes. 
            // We can grab the first element since it is the closest referenced element 
            sdf::ElementPtr closest_referenced_child_element = possibly_referenced_child_elements[0];
            if (closest_referenced_child_element->GetName() != "link")
            {
              std::cerr << "The child of the following joint is not a link:  " 
                          << this->GetSDFTreePathToElement(element) << std::endl;
              return pose;
            }
            else if (closest_referenced_child_element->HasAttribute("name") && closest_referenced_child_element->GetAttribute("name")->GetAsString() != "")
            {
              relative_to = closest_referenced_child_element->GetAttribute("name")->GetAsString();
            }
          }
          
          break;
        }
        child_element_ptr = child_element_ptr->GetNextElement("");
      }
      if (!has_child)
      {
        std::cerr << "No children specified for  " 
                    << this->GetSDFTreePathToElement(element) << std::endl;
        return pose;

      }
    }
    else
    {
      // There is no relative_to specification and none of the special cases applied
      // In this case we assume that pose is given relative to the nested scope
      
      // Use the scope as the implicit element which the pose is given relative to
      sdf::ElementPtr scope_element = element->GetParent();
      
      if (!scope_element)
      {
        // The current element is the root element

        // We are going to exit and end a series of recursive calls to this function.
        // In the scenario where a  previous recursive call was to canonical link of root element
        // AND the previously visited elements does not include the main model/world element (which
        // is the first child of the root element), we need to consider the main model/world element.
        // This might apply to actors and lights as well, but the 3D mesh parsing feature was not made for those
        
        if (sdf::ElementPtr top_level_model_or_world = element->GetFirstElement())
        {
          bool previously_visited = false;
          for (const auto& visited_element : previously_visited_elements)
          {
            if (visited_element.get() == top_level_model_or_world.get())
            {
              previously_visited = true;
            }
          }
          if (!previously_visited)
          {
            if (top_level_model_or_world->HasElement("pose"))
            {
              std::pair<glm::dvec3, glm::dquat> reference_pose =  this->ParsePoseElement(top_level_model_or_world->GetElement("pose"), relative_to);

              // Apply the rotation of the referenced pose to the position vector
              pose.first = reference_pose.second * pose.first;

              pose.first += reference_pose.first;
              pose.second = reference_pose.second * pose.second;
            }
          }
        }
        
        return pose;
      }
      else if (scope_element->HasAttribute("name") && scope_element->GetAttribute("name")->GetAsString() != "")
      {
        relative_to = scope_element->GetAttribute("name")->GetAsString();
      }
      else
      {
        std::cerr << "The scope element for the following element does not contain a valid name: " 
        << this->GetSDFTreePathToElement(element) << std::endl;
        return pose;
      }
    }
  }

  // At this point, we know that the pose is "relative_to" some other element
  // Determine what that element is (searching the current scope first)
  std::vector<sdf::ElementPtr> possibly_referenced_elements = this->LookupElementsInternal("name", relative_to,
                                              this->FindScope(element), nullptr);
  
  if (possibly_referenced_elements.size() <= 0)
  {
    std::cerr << "Could not find elements referenced by the pose of  " 
                << this->GetSDFTreePathToElement(element) << std::endl;
    return pose;
  }
  else
  {
    // LookupElementsInternal will return the referenced elements by order of when they were found in scopes. 
    // We can grab the first element since it is the closest referenced element 
    sdf::ElementPtr closest_referenced_element = possibly_referenced_elements[0];

    // If the element for which we have been trying to find the pose for so far is a 
    // canonical link or model, it may be that it's relative_to specification currently
    // references it's parent model, which is bound to lead to a cyclic reference.
    // A canonical link or model should be relative to it's enclosing scope. 
    // If the enclosing scope is a model, we should make sure that also is not a canonical model.
    sdf::ElementPtr canonical_element_of_referenced_element = this->FindCanonical(closest_referenced_element);
    if (canonical_element_of_referenced_element && canonical_element_of_referenced_element.get() == element.get())
    {
      bool current_element_is_canonical_link_or_model_of_reference_element = true;
      while (current_element_is_canonical_link_or_model_of_reference_element)
      {
        element = closest_referenced_element;
        closest_referenced_element = this->FindScope(closest_referenced_element);

        canonical_element_of_referenced_element = this->FindCanonical(closest_referenced_element);
        
        if (!canonical_element_of_referenced_element)
        {
          // Referenced element does not contain a canonical element, we're good
          current_element_is_canonical_link_or_model_of_reference_element = false;
        }
        else if (canonical_element_of_referenced_element.get() != element.get())
        {
          // Referenced element does contain a canonical element, but it's not equal to our current element
          current_element_is_canonical_link_or_model_of_reference_element = false;
        }
      }
    }

    for (const auto& visited_element : previously_visited_elements)
    {
      if (visited_element.get() == closest_referenced_element.get())
      {
        std::cerr << "Found a cyclic reference for " 
                  << this->GetSDFTreePathToElement(element) << std::endl;
        return pose;
      }
    }

    previously_visited_elements.push_back(closest_referenced_element);

    std::pair<glm::dvec3, glm::dquat> referenced_pose = this->FindAbsolutePose(closest_referenced_element, previously_visited_elements);

    // Apply the rotation of the referenced pose to the position vector
    pose.first = referenced_pose.second * pose.first;

    // Combine the poses
    pose.first += referenced_pose.first;
    pose.second = referenced_pose.second * pose.second;
  }

  return pose;
}


std::vector<double> SDFormatParser::ParseStringDoubleVector(const std::string& string_of_doubles, bool& success)
{
  std::vector<double> values;
  success = false;

  if (string_of_doubles.find_first_not_of("0123456789.-e ") != std::string::npos)
  {
    return values;
  }

  std::istringstream check_iss(string_of_doubles);
  std::string token;
  while (check_iss >> token)
  {
    if (std::count(token.begin(), token.end(), '.') > 1)
    {
      return values;
    }
    else if (token == "-")
    {
      return values;
    }
  }

  std::istringstream iss(string_of_doubles);
  double value;

  while (iss >> value)
  {
    values.push_back(value);
  }

  success = true;
  return values;
}


std::pair<glm::dvec3, glm::dquat> SDFormatParser::ParsePoseElement(sdf::ElementPtr element, std::string& relative_to)
{
  std::pair<glm::dvec3, glm::dquat> pose = std::make_pair(glm::dvec3(0.0f), glm::dquat());
  pose.second = glm::dquat(1.0, 0.0, 0.0, 0.0); // Identity quaternion

  if (element->HasAttribute("relative_to")) relative_to = element->GetAttribute("relative_to")->GetAsString();

  bool euler_rpy = true;

  if (element->HasAttribute("rotation_format")) 
  {
    if (element->GetAttribute("rotation_format")->GetAsString() == "euler_rpy")
    {
      euler_rpy = true;
    }
    else if (element->GetAttribute("rotation_format")->GetAsString() == "quat_xyzw")
    {
      euler_rpy = false;
    }
    else
    {
      std::cerr << "Invalid rotation format given for " << this->GetSDFTreePathToElement(element) << std::endl;
      return pose;
    }
  }

  if (element->GetValue()->GetAsString() == "") 
  { 
    // Empty pose element, assume the default that we already have
    return pose;
  }

  bool parsing_successful;

  std::vector<double> values = this->ParseStringDoubleVector(element->GetValue()->GetAsString(), parsing_successful);

  if (!parsing_successful)
  {
    std::cerr << "Invalid pose format for " << this->GetSDFTreePathToElement(element) << std::endl;
    return pose;
  }

  if (!euler_rpy)
  {
    // We expect a translation and a quaternion

    if (values.size() != 7)
    {
      std::cerr << "Invalid pose format for " << this->GetSDFTreePathToElement(element) << ". Expected 7 doubles for position and quaternion." << std::endl;
      return pose;
    }

    pose.first = glm::dvec3(values[0], values[1], values[2]);

    pose.second = glm::dquat(values[6], values[3], values[4], values[5]); // glm::dquat(w, x, y, z)
  }
  else if (euler_rpy)
  {
    // We expect a translation and a euler rpy

    if (values.size() != 6)
    {
      std::cerr << "Invalid pose format for " << this->GetSDFTreePathToElement(element) << ". Expected 6 doubles for position and rotation." << std::endl;
      return pose;
    }

    pose.first = glm::dvec3(values[0], values[1], values[2]);

    glm::dquat roll_quat = glm::angleAxis(values[3], glm::dvec3(1.0, 0.0, 0.0));
    glm::dquat pitch_quat = glm::angleAxis(values[4], glm::dvec3(0.0, 1.0, 0.0));
    glm::dquat yaw_quat = glm::angleAxis(values[5], glm::dvec3(0.0, 0.0, 1.0));

    pose.second = yaw_quat * pitch_quat * roll_quat;
  }

  return pose;
}

std::pair<std::vector<ModelViewerI::ModelInfo>, std::vector<ModelViewerI::PresetModelInfo>> SDFormatParser::GetModelsFromSDFTree(bool render_collisions)
{
  std::vector<ModelViewerI::ModelInfo> models;
  std::vector<ModelViewerI::PresetModelInfo> presetModels;

  if (!this->sdfElement) return std::pair(models, presetModels);

  std::vector<sdf::ElementPtr> model_defining_elements;
  
  std::vector<sdf::ElementPtr> visual_elements = this->LookupElementsByType("visual");
  model_defining_elements.insert(model_defining_elements.end(), visual_elements.begin(), visual_elements.end());
  
  if (render_collisions)
  {
    std::vector<sdf::ElementPtr> collision_elements = this->LookupElementsByType("collision");
    model_defining_elements.insert(model_defining_elements.end(), collision_elements.begin(), collision_elements.end());
  }
  
  for (sdf::ElementPtr model_defining_element : model_defining_elements) {
    std::pair<glm::dvec3, glm::dquat> absolute_pose = this->FindAbsolutePose(model_defining_element);
    sdf::ElementPtr geometry_element;
    double opacity = 1.0f;

    if (model_defining_element->GetName() == "collision")
    {
      opacity = 0.1f;
    }
    else if (model_defining_element->HasElement("transparency"))
    {
      bool success = false;
      std::vector<double> transparency_reading = ParseStringDoubleVector(model_defining_element->GetElement("transparency")->GetValue()->GetAsString(), success);
      if (!success || transparency_reading.size() != 1)
      {
        std::cerr << "Invalid transparency specified for " << this->GetSDFTreePathToElement(model_defining_element) << std::endl;
        continue;
      }
      else
      {
        opacity = 1 - transparency_reading[0];
      }
    }

    if ((geometry_element = model_defining_element->FindElement("geometry")))
    {
      sdf::ElementPtr geometry_child_element;
      if ((geometry_child_element = geometry_element->FindElement("box")) 
          || (geometry_child_element = geometry_element->FindElement("cylinder")) 
          || (geometry_child_element = geometry_element->FindElement("sphere")))
      {
        sdf::ElementPtr size_element;
        sdf::ElementPtr radius_element;
        sdf::ElementPtr length_element;
        ModelViewerI::PresetType preset_type = ModelViewerI::PresetType::BOX;
        glm::vec3 size_glm = {1.0f, 1.0f, 1.0f};
        bool success = true;
        if (geometry_child_element->GetName() == "box" && (size_element = geometry_child_element->FindElement("size")))
        {
          std::vector<double> box_size = ParseStringDoubleVector(size_element->GetValue()->GetAsString(), success);
          if (success == false || box_size.size() != 3)
          {
            std::cerr << "Invalid size specified for " << this->GetSDFTreePathToElement(size_element) << std::endl;
            continue;
          }
          size_glm = {box_size[0],box_size[1],box_size[2]};
          preset_type = ModelViewerI::PresetType::BOX;
        }
        else if (geometry_child_element->GetName() == "cylinder" && (radius_element = geometry_child_element->FindElement("radius")) && (length_element = geometry_child_element->FindElement("length")))
        {
          std::vector<double> cyl_radius = ParseStringDoubleVector(radius_element->GetValue()->GetAsString(), success);
          if (success == false || cyl_radius.size() != 1)
          {
            std::cerr << "Invalid specified for " << this->GetSDFTreePathToElement(radius_element) << std::endl;
            continue;
          }
          std::vector<double> cyl_len = ParseStringDoubleVector(length_element->GetValue()->GetAsString(), success);
          if (success == false || cyl_len.size() != 1)
          {
            std::cerr << "Invalid specified for " << this->GetSDFTreePathToElement(length_element) << std::endl;
            continue;
          }
          size_glm = {cyl_radius[0],cyl_radius[0],cyl_len[0]};
          preset_type = ModelViewerI::PresetType::CYLINDER;
        }
        else if (geometry_child_element->GetName() == "sphere" && (radius_element = geometry_child_element->FindElement("radius")))
        {
          std::vector<double> sphere_radius = ParseStringDoubleVector(radius_element->GetValue()->GetAsString(), success);
          if (success == false || sphere_radius.size() != 1)
          {
            std::cerr << "Invalid specified for " << this->GetSDFTreePathToElement(radius_element) << std::endl;
            continue;
          }
          size_glm = {sphere_radius[0],sphere_radius[0],sphere_radius[0]};
          preset_type = ModelViewerI::PresetType::SPHERE;
        }
        else
        {
          std::cerr << "Invalid geometry shape information for " << this->GetSDFTreePathToElement(geometry_element) << std::endl;
          continue;
        }
        
        ModelViewerI::PresetModelInfo model = {
          .preset_type = preset_type,
          .position = absolute_pose.first,
          .orientation = absolute_pose.second,
          .scale = size_glm,
          .opacity = opacity
        };
        presetModels.push_back(model);
      }
      else if ((geometry_child_element = geometry_element->FindElement("mesh")))
      {
        sdf::ElementPtr uri_element;
        if ((uri_element = geometry_child_element->FindElement("uri")))
        {
          std::string mesh_path = uri_element->GetValue()->GetAsString();
          if (std::filesystem::path(mesh_path).is_relative()) {
            std::string sdf_dir = std::filesystem::path(sdf_file_path).parent_path();
            mesh_path = sdf_dir + "/" + mesh_path;
          }
          ModelViewerI::ModelInfo model = {
            .model_absolute_path = mesh_path,
            .position = absolute_pose.first,
            .orientation = absolute_pose.second,
            .opacity = opacity
          };
          models.push_back(model);
        }
      }
    } 
    else
    {
      std::cerr << "Model defining element " << this->GetSDFTreePathToElement(model_defining_element) << " does not contain geometry element" << std::endl;
    }
  }

  return std::pair(models, presetModels);
}