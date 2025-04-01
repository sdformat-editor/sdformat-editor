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

#include <interfaces/sdformat_parser_interface.h>

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

    /// \brief Implementation of interface method 
    private: std::vector<ModelViewerI::ModelInfo> GetModelsFromSDFTree() override;
};

#endif