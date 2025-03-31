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

#ifndef SDFORMAT_EDITOR_ADD_ELEMENT_COMMAND_HH_
#define SDFORMAT_EDITOR_ADD_ELEMENT_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <interfaces/gui_interface.h>

#include <vector>

/// \brief Add Element command implementation of CommandI
class AddElementCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for add element command objects.
  /// \param[in] gui Pointer to the GUII object 
  /// \param[in] sdformatParser Pointer to the SDFormatParserI object
  /// \param[in] parent_element Pointer to the parent of the element that may be added
  /// \param[in] new_element Pointer to the element that is to be added
  public: AddElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr parent_element, sdf::ElementPtr new_element);

  /// \brief Implementation of interface method. 
  /// \returns Always true. Adds a new element to specified parent
  private: bool Execute() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the added element was removed from its parent
  private: bool ExecuteUndo() override;

  /// \brief Implementation of interface method. 
  /// \returns Returns true if the added element was restored to its parent again
  private: bool ExecuteRedo() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command as been executed or redone
  private: bool IsUndoable() override;

  /// \brief Implementation of interface method.
  /// \returns Returns true if the command has been undone
  private: bool IsRedoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool IsThreaded() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

  /// \brief Internal function to add element
  private: void AddElement(sdf::ElementPtr parent, sdf::ElementPtr element);

  /// @brief Pointer to the gui interface
  private: std::shared_ptr<GUII> gui;

  /// @brief Pointer to the sdformat parser interface
  private: std::shared_ptr<SDFormatParserI> sdformatParser;

  /// @brief Pointer to the parent of the element that may be added
  private: sdf::ElementPtr parent_element;

  /// @brief Pointer to the added element
  private: sdf::ElementPtr new_element;

  /// @brief Store if the command is currently undo-able
  private: bool is_currently_undoable = false;

  /// @brief Store if the command is currently redo-able
  private: bool is_currently_redoable = false;
};

#endif