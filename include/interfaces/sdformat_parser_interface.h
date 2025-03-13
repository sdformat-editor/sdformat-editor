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
// This interface depends on the SDFormat Library
#include <sdf/sdf.hh>

/// \brief Interface for the SDFormat Editor's Parser 
class SDFormatParserI
{
    /// \callgraph
    /// \brief Initialization of the Parser. Will re-initalize the parser if initalized already.
    /// NOTE: (zaid) As more features are added, more things will be added to this initalize method
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

    /// \brief Determines the locations of potential references to the element to delete
    /// \param[in] The key string to search for
    /// \returns The elements and attributes that mention this given key
    public: virtual Mentions FindMentions(std::string key) = 0;

    /// \brief Get the tree path to this element
    /// \param[in] The element that we want a tree path for
    /// \return Tree path as a string
    public: virtual std::string GetSDFTreePathToElement(sdf::ElementPtr element) = 0; 
    
};

#endif