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

#ifndef SDFORMAT_EDITOR_MODIFY_ELEMENT_COMMAND_HH_
#define SDFORMAT_EDITOR_MODIFY_ELEMENT_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>

#include <vector>

/// \brief Modify Element command implementation of CommandI
template <typename T>
class ModifyElementCommand : public CommandI
{
  /// \callgraph
  /// \brief Constructor for modify element command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  /// \param[in] element_to_modify Pointer to the element to modify
  /// \param[in] new_value The new value with which to modify the element
  public: ModifyElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr element_to_modify, T new_value);

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the element's value has been modified
  private: bool Execute() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the element's value has been reset 
  private: bool ExecuteUndo() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the element's value has been one again set to the new value
  private: bool ExecuteRedo() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command as been executed or redone
  private: bool IsUndoable() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command has been undone
  private: bool IsRedoable() override;

  /// \brief Implementation of interface method.
  /// \param[out] prevent_user_input indicates if user input should be prevented 
  /// if this happens to be a threaded command
  /// \returns Always false
  private: bool IsThreaded(bool& prevent_user_input) override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

  /// \brief Filepath of the model
  private: std::string file_path;

  /// @brief Pointer to the gui interface
  private: std::shared_ptr<GUII> gui;

  /// @brief Pointer to the sdformat parser interface
  private: std::shared_ptr<SDFormatParserI> sdformatParser;

  /// @brief Pointer to the element to modify
  private: sdf::ElementPtr element_to_modify;

  /// @brief The value that will be given to the element
  private: T new_value;

  /// @brief The value that will be overridden in the element
  private: T old_value;

  /// @brief Store if the command is currently undo-able
  private: bool is_currently_undoable = true;

  /// @brief Store if the command is currently redo-able
  private: bool is_currently_redoable = true;
};

#endif