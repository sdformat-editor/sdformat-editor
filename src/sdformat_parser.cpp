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
  
  // TEMPLATE CODE:

  // start parsing model
  const sdf::ElementPtr rootElement = sdfElement->Root();
  if (!rootElement->HasElement("model"))
  {
    std::cerr << file_path << " is not a model SDF file!" << std::endl;
    return;
  }

  const sdf::ElementPtr modelElement = rootElement->GetElement("model");
  const std::string modelName = modelElement->Get<std::string>("name");
  std::cout << "Found " << modelName << " model!" << std::endl;

  // parse model links
  sdf::ElementPtr linkElement = modelElement->GetElement("link");
  while (linkElement)
  {
    const std::string linkName = linkElement->Get<std::string>("name");
    std::cout << "Found " << linkName << " link in "
              << modelName << " model!" << std::endl;
    linkElement = linkElement->GetNextElement("link");
  }

  // parse model joints
  sdf::ElementPtr jointElement = modelElement->GetElement("joint");
  while (jointElement)
  {
    const std::string jointName = jointElement->Get<std::string>("name");
    std::cout << "Found " << jointName << " joint in "
              << modelName << " model!" << std::endl;

    const sdf::ElementPtr parentElement = jointElement->GetElement("parent");
    const std::string parentLinkName = parentElement->Get<std::string>();

    const sdf::ElementPtr childElement = jointElement->GetElement("child");
    const std::string childLinkName = childElement->Get<std::string>();

    std::cout << "Joint " << jointName << " connects " << parentLinkName
              << " link to " << childLinkName << " link" << std::endl;

    jointElement = jointElement->GetNextElement("joint");
  }
  success = true;
}