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
#include <mutex>

#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/command_interface.h>

/// \brief Interface for the SDFormat Editor's Graphical User Interface 
/// Note that this class also inherits from std::enable_shared_from_this<GUII>, 
/// as it may be required to pass a shared pointer of itself into the CommandI
/// object in the Update method.
class GUII : public std::enable_shared_from_this<GUII>
{
    /// \callgraph
    /// \brief Initialization of the GUI. Should be wrapped in the constructor of the implementation.
    /// NOTE: (zaid) As more features are added, more things will be added to this initalize method
    /// \param[in] window_name The name to be given to the SDFormatEditor Window
    /// \param[in] sdformat_parser Pointer to the sdformat parser object
    /// \param[out] success true if window initalization is successful
    protected: virtual void Initialize(const std::string &windowName, std::shared_ptr<SDFormatParserI> sdformat_parser, bool &success) = 0;

    /// \callgraph
    /// \brief Updating the GUI
    /// \returns The a pointer to the command resulting from the user's action during this frame
    public: virtual std::unique_ptr<CommandI> Update() = 0;

    /// \callgraph
    /// \brief Indicate if the GUI should close 
    public: virtual bool ShouldClose() = 0;

    /// \callgraph
    /// \brief Open the file dialog
    /// \returns An file path or ""
    public: virtual std::string OpenFileDialog() = 0;

    /// \callgraph
    /// \brief Method to change the flag which can be set to prevent the GUI from taking user input.
    /// This can be used in instances where OpenFileDialog or another threadded method is running.
    /// \param[in] set value to set the flag
    public: virtual void set_prevent_input_flag(bool set) = 0;

    /// \callgraph
    /// \brief Method to lock mutex
    public: virtual std::unique_lock<std::mutex> lock_mutex() = 0;

};;

#endif