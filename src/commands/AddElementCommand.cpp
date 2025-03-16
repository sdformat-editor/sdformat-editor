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

#include "commands/AddElementCommand.h"

AddElementCommand::AddElementCommand(std::shared_ptr<GUII> gui, 
                                        std::shared_ptr<SDFormatParserI> sdformatParser,
                                        sdf::ElementPtr parent_element,
                                        sdf::ElementPtr new_element)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
    this->parent_element = parent_element;
    this->new_element = new_element;
}

bool AddElementCommand::Execute()
{
    if (!new_element)
    {
        return false;
    }
    else
    {
        this->new_element->SetParent(this->parent_element);
        this->parent_element->InsertElement(this->new_element);
    }

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool AddElementCommand::ExecuteUndo()
{
    
    this->new_element->RemoveFromParent();

    this->is_currently_undoable = false;
    this->is_currently_redoable = true;

    return true;
}

bool AddElementCommand::ExecuteRedo()
{
    this->new_element->SetParent(this->parent_element);
    this->parent_element->InsertElement(this->new_element);

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool AddElementCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool AddElementCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool AddElementCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool AddElementCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}