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

#include "commands/AddAttributeCommand.h"

AddAttributeCommand::AddAttributeCommand(std::shared_ptr<GUII> gui, 
                                        std::shared_ptr<SDFormatParserI> sdformatParser,
                                        sdf::ElementPtr parent_element,
                                        sdf::ParamPtr new_attribute)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
    this->parent_element = parent_element;
    this->new_attribute = new_attribute;
}

bool AddAttributeCommand::Execute()
{
    if (!new_attribute)
    {
        std::cout << "Adding custom attributes is not implemented" << std::endl;
        return false;
    }
    else
    {
        this->new_attribute->SetParentElement(this->parent_element);
        this->parent_element->AddAttribute(this->new_attribute->GetKey(), this->new_attribute->GetTypeName(), "", false);
        // The third and forth parameters are placeholders
    }

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool AddAttributeCommand::ExecuteUndo()
{
    
    this->parent_element->RemoveAttribute(this->new_attribute->GetKey());

    this->is_currently_undoable = false;
    this->is_currently_redoable = true;

    return true;
}

bool AddAttributeCommand::ExecuteRedo()
{
    this->new_attribute->SetParentElement(this->parent_element);
    this->parent_element->AddAttribute(this->new_attribute->GetKey(), this->new_attribute->GetTypeName(), "", false);

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool AddAttributeCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool AddAttributeCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool AddAttributeCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool AddAttributeCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}