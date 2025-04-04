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

#ifndef SDFORMAT_EDITOR_PARSER_HH_
#define SDFORMAT_EDITOR_PARSER_HH_

#include <cstdlib>
#include <cstring>
#include <limits.h>
#include <memory>
#include <filesystem>

#include <interfaces/sdformat_parser_interface.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>

/// \brief Implementation of SDFormatParserI
class SDFormatParser : public SDFormatParserI
{

    /// \brief The main sdfElement associated this this SDFormatParser instance
    private: sdf::SDFPtr sdfElement;

    /// \brief Implementation of interface method
    private: void Initialize(const std::string file_path, bool &success) override;

    /// \brief Implementation of interface method
    /// \returns The main sdfElement associated this this SDFormatParser instance
    private: sdf::SDFPtr GetSDFElement() override;

    /// \brief Implementation of interface method
    /// \param[in] attribute_type The attribute type to use for lookup
    /// \returns A list of elements with a given attribute type
    private: std::vector<sdf::ElementPtr> LookupElementsByAttributeType(const std::string& attribute_type) override;

    /// \brief Implementation of interface method
    /// \param[in] attribute_type The attribute type to use for lookup
    /// \param[in] attribute_value The attribute type to use for lookup
    /// \returns A list of elements with the given attribute type and the given attribute value
    private: std::vector<sdf::ElementPtr> LookupElementsByAttributeTypeAndValue(const std::string& attribute_type, const std::string& attribute_value) override;

    /// \brief Attempts to find an element containing the given attribute type and attribute value within an inital scope. 
    /// The method will continue expanding the scope until it reaches the root element 
    /// \param[in] attribute_type The attribute type to use for lookup
    /// \param[in] attribute_value The attribute type to use for lookup
    /// \param[in] scope The initial search scope. Must be a model or world element
    /// \param[in] scope_to_exclude The scope to exclude. Must be a model or world element
    /// \returns A list of elements with the given attribute type and the given attribute value, ordered by scope
    private: std::vector<sdf::ElementPtr> LookupElementsInternal(const std::string& attribute_type, const std::string& attribute_value, 
                                                                                    sdf::ElementPtr scope, sdf::ElementPtr scope_to_exclude);
    
    /// \brief Finds the nearest world, model, or root element parent of the given element
    /// \param[in] element we want to get the scope for
    /// \return The world, model, or root element defining the given element's scope
    private: sdf::ElementPtr FindScope(sdf::ElementPtr element); 

    /// \brief Finds the canonical link of a model and, if that doesn't exist, the first child nested model 
    /// \param[in] element we want to the "canonical" of
    /// \return The canonical link. If that does not exist in the immediate scope, the first nested model. 
    /// If that does not exist, a null pointer 
    private: sdf::ElementPtr FindCanonical(sdf::ElementPtr element); 

    /// \brief Implementation of interface method
    /// \param[in] type The type by which to lookup elements
    /// \returns A list of elements with a given attribute type
    private: std::vector<sdf::ElementPtr> LookupElementsByType(const std::string& type) override;

    /// \brief Implementation of interface method
    /// \param[in] key the string to search for
    /// \returns The elements and attributes that mention this given key
    private: Mentions FindMentions(std::string key) override;

    /// \brief Implementation of interface method
    /// \param[in] key the string to search for
    /// \param[in] element_to_exclude the element to exclude in the search
    /// \returns The elements and attributes that mention this given key
    private: Mentions FindMentions(std::string key, sdf::ElementPtr element_to_exclude) override;

    /// \brief Implementation of interface method
    /// \param[in] key the string to search for
    /// \param[in] attribute_to_exclude the attribute to exclude in the search
    /// \returns The elements and attributes that mention this given key
    private: Mentions FindMentions(std::string key, sdf::ParamPtr attribute_to_exclude) override;

    /// \brief Implementation of interface method
    /// \param[in] key the string to search for
    /// \param[in] element_to_exclude the element to exclude in the search; Can be nullptr
    /// \param[in] attribute_to_exclude the attribute to exclude in the search; Can be nullptr
    /// \returns The elements and attributes that mention this given key
    private: Mentions FindMentions(std::string key, sdf::ElementPtr element_to_exclude, sdf::ParamPtr attribute_to_exclude) override;

    /// \brief Implementation of interface method
    /// \param[in] element element that we want a tree path for
    /// \return Tree path as a string
    private: std::string GetSDFTreePathToElement(sdf::ElementPtr element) override; 

    /// \brief Finds the absolute pose (translation and rotation) of a given element which contains pose element
    /// \param[in] element The element for which we want to find absolute position
    /// \param[in] previously_visited_elements indicates elements that were previously visited as we are trying to compute absolute pose. 
    ///             Used to detect a reference cycle in "relative_to" mentions.
    /// \returns A pair datatype containing the absolute translation and rotation of the given element 
    private: std::pair<glm::dvec3, glm::dquat> FindAbsolutePose(sdf::ElementPtr element, std::vector<sdf::ElementPtr> previously_visited_elements = {});

    /// \brief Parses a given pose element to get the pose values, and also indicates if this pose is relative to anything else
    /// \param[in] element The pose element which we want to parse
    /// \param[out] relative_to The name of the element that the pose is given relative to
    /// \returns A pair datatype containing the translation and quaternion rotation of the given element
    /// The translations and rotations will all be zero if the given element is not a pose element 
    private: std::pair<glm::dvec3, glm::dquat> ParsePoseElement(sdf::ElementPtr element, std::string& relative_to);

    /// \brief Considers if this is a special case of "relative_to" and approperiately handles it
    /// \param[in] element The non-pose element which we want to parse
    /// \param[out] relative_to The "relative_to" specification
    /// \returns Returns true if successful
    private: bool HandleRelativeToSpecificationSpecialcases(sdf::ElementPtr element, std::string& relative_to);

    /// \brief Given a string containing doubles seperated by spaces, this method returns a vector a doubles
    /// \param[in] string_of_doubles
    /// \param[out] success
    /// \returns Returns a vector of doubles
    private: std::vector<double> ParseStringDoubleVector(const std::string& string_of_doubles, bool& success);

    /// \brief Implementation of interface method 
    private: std::pair<std::vector<ModelViewerI::ModelInfo>, std::vector<ModelViewerI::PresetModelInfo>> GetModelsFromSDFTree(bool render_collisions = false) override;

    /// \brief Store the file path of the sdf file
    private: std::string sdf_file_path;
};

#endif