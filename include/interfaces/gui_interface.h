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

#ifndef SDFORMAT_EDITOR_GUI_INTERFACE_HH_
#define SDFORMAT_EDITOR_GUI_INTERFACE_HH_

#include <iostream>
#include <atomic>
#include <memory>

#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/command_interface.h>

/// \brief Interface for the SDFormat Editor's Graphical User Interface 
class GUII
{

/// \brief Initialization of the GUI. Should be wrapped in the constructor of the implementation.
/// NOTE: (zaid) As more features are added, more things will be added to this initalize method
/// \param[in] window_name The name to be given to the SDFormatEditor Window
/// \param[out] success true if window initalization is successful
protected: virtual void Initialize(const std::string &windowName, bool &success) = 0;

/// \brief Updating the GUI
/// \param[in] sdfParser Pointer to the SDF parser interface object
/// \returns The a pointer to the command resulting from the user's action during this frame
public: virtual std::unique_ptr<CommandI> Update(SDFormatParserI* sdformat_parser) = 0;

/// \brief Indicate if the GUI should close 
public: virtual bool ShouldClose() = 0;

/// \brief Open the file dialog
/// \returns An file path or ""
public: virtual std::string OpenFileDialog() = 0;

/// \brief A flag which can be set to prevent the GUI from taking user input.
/// This can be used in instances where OpenFileDialog or another threadded method is running.
/// NOTE: (zaid) Technically, this class is no longer an "interface" since this attribute is included...
public: std::atomic<bool> prevent_input_flag = false;

};

#endif