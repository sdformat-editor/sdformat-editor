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

#ifndef SDFORMAT_EDITOR_MAIN_HH_
#define SDFORMAT_EDITOR_MAIN_HH_

#include <stdio.h>
#include "gui.h"
#include "sdformat_parser.h"
#include "interfaces/command_interface.h"

/// \brief Implementation of program start up to be called by main.
class FileEditorRunner
{
    /// \brief Constructor that calls the top level method
    public: FileEditorRunner();

    /// \brief Top level method that starts the program
    /// \return Exit state for main function
    public: int run_program();

    /// @brief Variable for checking if the gui was initilized successfully
    bool gui_initalization_successful;

    /// @brief Pointer to SDFormatParser which will be null until the user opens a file
    std::shared_ptr<SDFormatParserI> sdformatParser;

    /// @brief Pointer to the GUI class
    std::shared_ptr<GUII> gui;

    /// @brief Stack for undo functionality
    std::vector<std::unique_ptr<CommandI>> undo_commands_stack;

    /// @brief Stack for redo functionality
    std::vector<std::unique_ptr<CommandI>> redo_commands_stack;

};

#endif
