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

#ifndef SDFORMAT_EDITOR_PARSER_INTERFACE_HH_
#define SDFORMAT_EDITOR_PARSER_INTERFACE_HH_

#include <iostream>

// SDFormat dependencies
#include <sdf/sdf.hh>

#include <interfaces/model_viewer_interface.h>

/// \brief Interface for the SDFormat Editor's Parser 
class SDFormatParserI
{
    /// \callgraph
    /// \brief Initialization of the Parser. Will re-initalize the parser if initalized already.
    /// \param[in] file_path The relative or absolute filepath to the SDF model. If relative, a relative_to path must be specified.
    /// \param[out] success True if SDFParser initalization is successful
    public: virtual void Initialize(const std::string file_path, bool &success) = 0;

    /// \callgraph
    /// \brief Returns the main sdfElement associated this this SDFormatParser instance
    /// \returns the main sdfElement associated this this SDFormatParser instance
    public: virtual sdf::SDFPtr GetSDFElement() = 0;

    /// @brief Struct to define a "mentions" object type
    public: struct Mentions
    {
        std::vector<sdf::ElementPtr> elements;
        std::vector<sdf::ParamPtr> attributes;
    };

    /// \brief Provides a list of elements which have an attribute of the given attribute type
    /// \param[in] attribute_type The attribute type to use for lookup
    /// \returns A list of elements with a given attribute type
    public: virtual std::vector<sdf::ElementPtr> LookupElementsByAttributeType(const std::string& attribute_type) = 0;

    /// \brief Provides a list of elements which have an attribute of the given attribute type and the given attribute value
    /// \param[in] attribute_type The attribute type to use for lookup
    /// \param[in] attribute_value The attribute type to use for lookup
    /// \returns A list of elements with the given attribute type and the given attribute value
    public: virtual std::vector<sdf::ElementPtr> LookupElementsByAttributeTypeAndValue(const std::string& attribute_type, const std::string& attribute_value) = 0;

    /// \brief Provides a list of elements which are of a given type
    /// \param[in] type The type by which to lookup elements
    /// \returns A list of elements with a given attribute type
    public: virtual std::vector<sdf::ElementPtr> LookupElementsByType(const std::string& type) = 0;

    /// \brief Finds the elements and attributes which mention a given key
    /// \param[in] key the string to search for
    /// \returns The elements and attributes that mention the given key
    public: virtual Mentions FindMentions(std::string key) = 0;
    
    /// \brief Finds the elements and attributes which mention a given key
    /// \param[in] key the string to search for
    /// \param[in] element_to_exclude the element to exclude in the search
    /// \returns The elements and attributes that mention the given key
    public: virtual Mentions FindMentions(std::string key, sdf::ElementPtr element_to_exclude) = 0;

    /// \brief Finds the elements and attributes which mention a given key
    /// \param[in] key the string to search for
    /// \param[in] attribute_to_exclude the attribute to exclude in the search
    /// \returns The elements and attributes that mention the given key
    public: virtual Mentions FindMentions(std::string key, sdf::ParamPtr attribute_to_exclude) = 0;

    /// \brief Finds the elements and attributes which mention a given key
    /// \param[in] key the string to search for
    /// \param[in] element_to_exclude the element to exclude in the search
    /// \param[in] attribute_to_exclude the attribute to exclude in the search
    /// \returns The elements and attributes that mention the given key
    public: virtual Mentions FindMentions(std::string key, sdf::ElementPtr element_to_exclude, sdf::ParamPtr attribute_to_exclude) = 0;

    /// \brief Get the tree path to a given element
    /// \param[in] element element that we want a tree path for
    /// \return Tree path as a string
    public: virtual std::string GetSDFTreePathToElement(sdf::ElementPtr element) = 0; 

    /// \brief Gets information about all models in the sdf tree in a format that can be interperted by the model viewer
    /// \param[in] render_collisions indicates if collision meshes and shapes are included as well
    /// \return A pair of vectors condaining structs of the model viewer interfaces's ModelInfo and Preset Model Info, which in turn contain the necessary information
    /// to render models
    public: virtual std::pair<std::vector<ModelViewerI::ModelInfo>, std::vector<ModelViewerI::PresetModelInfo>> GetModelsFromSDFTree(bool render_collisions = false) = 0;

};

#endif