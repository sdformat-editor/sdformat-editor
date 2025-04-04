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
#include "file_operations.h"
#include "model_viewer.h"
#include <sys/stat.h>
#include <fstream>
#include <thread>

/// \brief Class to manage overall program operation.
class FileEditorRunner
{
    /// \brief Constructor that calls the top level method
    /// \param[in] data_dir_created boolean that communicates if the file to contain the previously opened model exists
    public: FileEditorRunner(bool data_dir_created = false);

    /// \brief Top level method that starts the program
    /// \return The program's exit state 
    public: int RunProgram();
    
    /// @brief Variable for checking if the gui was initilized successfully
    bool gui_initalization_successful;

    /// @brief Variable for checking if the model viewer was initilized successfully
    bool model_viewer_initalization_successful;

    /// @brief Pointer to SDFormatParser which will be null until the user opens a file
    std::shared_ptr<SDFormatParserI> sdformatParser;

    /// @brief Pointer to the GUI class instance
    std::shared_ptr<GUII> gui;

    /// @brief Pointer to the CommandFactory class instance
    std::shared_ptr<CommandFactoryI> command_factory;

    /// @brief Pointer to the model viewer object
    std::shared_ptr<ModelViewerI> model_viewer;

};

#endif
