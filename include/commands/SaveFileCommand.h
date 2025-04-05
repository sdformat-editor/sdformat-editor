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

#ifndef SDFORMAT_EDITOR_SAVE_FILE_COMMAND_HH_
#define SDFORMAT_EDITOR_SAVE_FILE_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>


/// \brief Save model command implementation of CommandI
class SaveFileCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for save model command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  public: SaveFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser);

  /// \brief Implementation of interface method. 
  /// \returns True if the file has been successfully saved
  private: bool Execute();

  /// \brief Implementation of interface method. 
  /// \returns Always false
  private: bool ExecuteUndo() override;

  /// \brief Implementation of interface method. 
  /// \returns Always false
  private: bool ExecuteRedo() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool IsUndoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool IsRedoable() override;

  /// \brief Implementation of interface method.
  /// \param[out] prevent_user_input indicates if user input should be prevented 
  /// if this happens to be a threaded command
  /// \returns Always false
  private: bool IsThreaded(bool& prevent_user_input) override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

  /// @brief Pointer to the gui interface
  private: std::shared_ptr<GUII> gui;

  /// @brief Pointer to the sdformat parser interface
  private: std::shared_ptr<SDFormatParserI> sdformatParser;

};

#endif