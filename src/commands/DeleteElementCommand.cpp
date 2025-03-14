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

#include "commands/DeleteElementCommand.h"

DeleteElementCommand::DeleteElementCommand(std::shared_ptr<GUII> gui, 
                                        std::shared_ptr<SDFormatParserI> sdformatParser,
                                        sdf::ElementPtr element_to_delete)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
    this->element_to_delete = element_to_delete;
}

bool DeleteElementCommand::Execute()
{

    SDFormatParserI::Mentions mentions;

    // Find mentions of the element to deleting, excluding the element itself 
    if (auto name_attribute = this->element_to_delete->GetAttribute("name"))
    {
        mentions = this->sdformatParser->FindMentions(name_attribute->GetAsString(), this->element_to_delete);
    }

    // If this element has mentions anywhere else OR it is a required attributes, 
    // display an "are you sure?" message to the user
    if (!mentions.attributes.empty() || !mentions.elements.empty() || this->element_to_delete->GetRequired() == "1" || this->element_to_delete->GetRequired() == "+")
    {

        const std::string dialog_message_header = "Warning";
        const std::string dialog_message_footer = "Are you sure you want to continue?";
        std::string dialog_message_body;

        if (this->element_to_delete->GetRequired() == "1" || this->element_to_delete->GetRequired() == "+")
        {
            dialog_message_body += "Element "  + this->sdformatParser->GetSDFTreePathToElement(this->element_to_delete) + " is a required element.\n\n";
        }

        if (!mentions.elements.empty())
        {
            dialog_message_body += "The following elements mention this element: \n";
            for (const auto &element_mention : mentions.elements)
            {
                dialog_message_body += "- " + this->sdformatParser->GetSDFTreePathToElement(element_mention) + "\n";
            }
        }

        if (!mentions.attributes.empty())
        {
            dialog_message_body += "The following attributes mention this element: \n";
            for (const auto &attribute_mention : mentions.attributes)
            {
                dialog_message_body += "- Attribute " + attribute_mention->GetKey() + " of " + this->sdformatParser->GetSDFTreePathToElement(attribute_mention->GetParentElement()) + "\n";
            }
        }

        GUII::DialogMessage dialog_message{dialog_message_header, dialog_message_body, dialog_message_footer};

        // Display the "Are you sure?" dialog
        if (!this->gui->OpenYesNoDialog(dialog_message))
        {
            // User decides to not proceed. 
            // The command does not need to do anything and can finish executing.
            return true;
        }
    }

    // Store the parent
    this->element_to_deletes_parent = element_to_delete->GetParent();
    
    // Remove the element to delete from it's parent
    element_to_delete->RemoveFromParent();

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool DeleteElementCommand::ExecuteUndo()
{
    element_to_deletes_parent->InsertElement(element_to_delete);

    this->is_currently_undoable = false;
    this->is_currently_redoable = true;

    return true;
}

bool DeleteElementCommand::ExecuteRedo()
{
    // Store the parent
    this->element_to_deletes_parent = element_to_delete->GetParent();
    
    // Remove the element to delete from it's parent
    element_to_delete->RemoveFromParent();

    // Flag the command as "undo-able"    
    this->is_currently_undoable = true;
    this->is_currently_redoable = false;

    return true;
}

bool DeleteElementCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool DeleteElementCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool DeleteElementCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool DeleteElementCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}