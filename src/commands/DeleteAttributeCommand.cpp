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

#include "commands/DeleteAttributeCommand.h"

DeleteAttributeCommand::DeleteAttributeCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ParamPtr attribute_to_delete) : gui(gui), sdformatParser(sdformatParser), attribute_to_delete(attribute_to_delete) {}

bool DeleteAttributeCommand::Execute()
{
    this->parent_element = attribute_to_delete->GetParentElement();

    // Remove the element to delete from it's parent
    this->parent_element->RemoveAttribute(attribute_to_delete->GetKey());

    // Flag the command as "undo-able"
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool DeleteAttributeCommand::ExecuteUndo()
{
    parent_element->AddAttribute(
        attribute_to_delete->GetKey(),
        attribute_to_delete->GetTypeName(),
        attribute_to_delete->GetDefaultAsString(),
        attribute_to_delete->GetRequired(),
        attribute_to_delete->GetDescription());

    this->is_currently_undoable = false;
    this->is_currently_redoable = true;

    return true;
}

bool DeleteAttributeCommand::ExecuteRedo()
{
    this->Execute();

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool DeleteAttributeCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool DeleteAttributeCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool DeleteAttributeCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool DeleteAttributeCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}