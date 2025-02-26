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

/// \brief Interface for the SDFormat Editor's Parser 
class SDFormatParserI
{

/// \brief Initialization of the Parser. Should be wrapped in the constructor of the implementation.
/// NOTE: (zaid) As more features are added, more things will be added to this initalize method
/// \param[in] file_path The relative or absolute filepath to the SDF model. If relative, a relative_to path must be specified.
/// \param[out] success True if SDFParser initalization is successful
/// \param[in] is_relative Optional. If true, indicates the provided path is relative
/// \param[in] relative_to Optional. If is_relative is true, this must be an absolute path specifying what file_path is relative to.
protected: virtual void Initialize(const std::string file_path, bool &success, const bool is_relative = false, const std::string relative_to = "") = 0;

};

#endif