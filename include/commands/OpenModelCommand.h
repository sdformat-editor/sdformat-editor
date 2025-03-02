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

#ifndef SDFORMAT_EDITOR_ADD_MODEL_COMMAND_HH_
#define SDFORMAT_EDITOR_ADD_MODEL_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>


/// \brief Open model command implementation of CommandI
class OpenModelCommand : public CommandI
{

  /// \brief Constructor for open model command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  /// \param[in] file_path File path the model
  public: OpenModelCommand(GUII* gui, SDFormatParserI* sdformatParser, const std::string &file_path);

  /// \brief Opens the given file path using the sdformat parser and displays the result in the GUI 
  /// \returns True if the SDFormatParser has successfully parsed the file and it has been displayed in the GUI
  private: bool execute() override;

  /// \brief The open model command should not be undoable
  /// \returns Always False
  private: bool undo() override;

  /// \brief The open model command should not be redoable
  /// \returns Always False
  private: bool redo() override;

  /// \brief Destructor 
  public: ~OpenModelCommand();

  /// \brief Filepath of the model
  private: const std::string &file_path;

  /// @brief Pointer to the gui interface
  private: GUII* gui;

  /// @brief Pointer to the sdformat parser interface
  private: SDFormatParserI* sdformatParser;

};

#endif