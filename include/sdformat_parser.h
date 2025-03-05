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

    
};

#endif