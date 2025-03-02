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

    /// \brief Execution of the command. Must be implemented as applicable by the derived class.
    virtual void execute() = 0;

    /// \brief Undo functionality of the command.
    /// \param[out] success True if the command can be and has been undone
    virtual void undo(bool &success) = 0;

    /// \brief Redo functionality of the command.
    /// \param[out] success True if the command can be and has been redone
    virtual void redo(bool &success) = 0;

};

#endif