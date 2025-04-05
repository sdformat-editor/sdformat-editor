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

#ifndef SDFORMAT_EDITOR_COMMAND_INTERFACE_HH_
#define SDFORMAT_EDITOR_COMMAND_INTERFACE_HH_

#include <iostream>

/// \brief The command class encapsulates the interactions
/// between the various interfaces of the SDFormat Editor,
/// providing undo/redo functionality when applicable
class CommandI
{
    /// \callgraph
    /// \brief Execution of the command. Must be implemented as applicable by the derived class.
    /// \returns True if the command has been executed
    public: virtual bool Execute() = 0;

    /// \callgraph
    /// \brief Execution of the undo command. Must be implemented as applicable by the derived class.
    /// \returns True if the command undo-able and has been undone
    public: virtual bool ExecuteUndo() = 0;

    /// \callgraph
    /// \brief Execution of the redo command. Must be implemented as applicable by the derived class.
    /// \returns True if the command is redo-able and has been redone
    public: virtual bool ExecuteRedo() = 0;

    /// \callgraph
    /// \brief Undo functionality of the command.
    /// \returns True if the command can be undone
    public: virtual bool IsUndoable() = 0;

    /// \callgraph
    /// \brief Redo functionality of the command.
    /// \returns True if the command can be redone
    public: virtual bool IsRedoable() = 0;

    /// \callgraph
    /// \brief Indicates if this command should be run in a thread
    /// \param[out] prevent_user_input indicates if user input should be prevented 
    /// if this happens to be a threaded command
    /// \returns True if this command should be run in a thread
    public: virtual bool IsThreaded(bool& prevent_user_input) = 0;

    /// \callgraph
    /// \brief Indicates if this command is an irreversible state-changing command.
    /// \returns True if this command changes the state of the program irreversibly
    public: virtual bool ChangesProgramStateIrreversibly() = 0;
};;

#endif