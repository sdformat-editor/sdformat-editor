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

#ifndef SDFORMAT_EDITOR_OPEN_FILE_COMMAND_HH_
#define SDFORMAT_EDITOR_OPEN_FILE_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>


/// \brief Open model command implementation of CommandI
class OpenFileCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for open model command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  public: OpenFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser);

  /// \brief Implementation of interface method. 
  /// \returns True if the SDFormatParser has successfully parsed the file and it has been displayed in the GUI
  private: bool execute() override;

  /// @brief Implementation of interface method.
  /// @return Always false
  private: bool execute_undo() override;

  /// @brief Implementation of interface method.
  /// @return Always false
  private: bool execute_redo() override;

  /// \brief Implementation of interface method.
  /// \returns Always False
  private: bool is_undoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool is_redoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always true
  private: bool is_threaded() override;

  /// \brief Filepath of the model
  private: std::string file_path;

  /// @brief Pointer to the gui interface
  private: std::shared_ptr<GUII> gui;

  /// @brief Pointer to the sdformat parser interface
  private: std::shared_ptr<SDFormatParserI> sdformatParser;

};

#endif