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

#ifndef COMMAND_FACTORY_INTERFACE_HH_
#define COMMAND_FACTORY_INTERFACE_HH_

// SDFormat dependencies
#include <sdf/sdf.hh>
#include <memory>
#include "interfaces/command_interface.h"
#include "interfaces/gui_interface.h"
#include "interfaces/sdformat_parser_interface.h"
#include <stack>

// Predeclare GUII to avoid circular dependencies
class GUII;

/// \brief Interface for the SDFormat Editor's Command Factory
class CommandFactoryI
{
  /// \callgraph
  /// \brief Initialization of the Command Factory. Should be wrapped in the constructor of the implementation.
  /// \param[in] gui A pointer to the GUI element
  /// \param[in] sdformat_parser A pointer to the sdformat parser object
  private: virtual void Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser) = 0;

  /// \brief Create an open file command
  /// \return Unique pointer to a command interface 
  public: virtual std::unique_ptr<CommandI> MakeOpenFileCommand(std::string file_path) = 0;

  /// \brief Create a delete element command
  /// \param[in] element_to_delete The SDF element to delete
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeDeleteElementCommand(
      sdf::ElementPtr element_to_delete) = 0;

  /// \brief Create a undo command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeUndoCommand() = 0;
  
  /// \brief Create a redo command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeRedoCommand() = 0;

  /// \brief Create a save file command
  /// \param force_save_as_not_save TODO: description (evan)
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeSaveFileCommand(bool force_save_as_not_save) = 0;

  /// \brief Pushes to the undo commands stack
  /// \param[in] command commandI object to push
  /// \param[in] new_change indicates if we are pushing a new change, and thus should clear the redo stack
  public: virtual void PushToUndoCommandsStack(std::unique_ptr<CommandI> command, const bool new_change = true) = 0;

  /// \brief Pushes to the redo commands stack
  /// \param[in] command commandI object to push
  public: virtual void PushToRedoCommandsStack(std::unique_ptr<CommandI> command) = 0;

  /// \brief Clears both the undo and redo stacks
  public: virtual void ClearUndoRedoStacks() = 0;
};

#endif // COMMAND_FACTORY_INTERFACE_HH_