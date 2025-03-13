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

#ifndef SDFORMAT_EDITOR_DELETE_ELEMENT_COMMAND_HH_
#define SDFORMAT_EDITOR_DELETE_ELEMENT_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>

#include <vector>

/// \brief Open model command implementation of CommandI
class DeleteElementCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for open model command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  public: DeleteElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr element_to_delete);

  /// \brief Implementation of interface method. 
  /// \returns Always true. Removes the element to delete from it's parent
  private: bool execute() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the element to delete was already deleted and now restored to its parent
  private: bool execute_undo() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the element to delete was removed from its parent again
  private: bool execute_redo() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command as been executed or redone
  private: bool is_undoable() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command has been undone
  private: bool is_redoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool is_threaded() override;

  /// \brief Filepath of the model
  private: std::string file_path;

  /// @brief Pointer to the gui interface
  private: std::shared_ptr<GUII> gui;

  /// @brief Pointer to the sdformat parser interface
  private: std::shared_ptr<SDFormatParserI> sdformatParser;

  /// @brief Pointer to the element to delete
  private: sdf::ElementPtr element_to_delete;

  /// @brief Pointer to the parent of the element to delete
  private: sdf::ElementPtr element_to_deletes_parent;

  /// @brief Store if the command is currently undo-able
  private: bool is_currently_undoable = false;

  /// @brief Store if the command is currently redo-able
  private: bool is_currently_redoable = false;
};

#endif