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

    bool element_required = this->ElementRequired();

    // If this element has mentions anywhere else OR it is a required attributes, 
    // display a choice dialog to the user
    if (!mentions.attributes.empty() || !mentions.elements.empty() || element_required)
    {

        const std::string dialog_message_header = "Warning";
        const std::string dialog_message_footer = "Are you sure you want to continue?";
        std::string dialog_message_body;

        std::vector<std::pair<std::string, bool>> user_choices;

        user_choices.push_back({"Proceed", false});
        user_choices.push_back({"Cancel", false});

        if (element_required)
        {
            dialog_message_body += "Element "  + this->sdformatParser->GetSDFTreePathToElement(this->element_to_delete) 
                                    + " is a required element due to its parent! Deleting it will render this SDF invalid.\n\n";
            
            // Modify the user choices based on this scenario
            user_choices[0].first = "Proceed anyway (break the sdf)";
            user_choices.push_back({"Delete the element's parent", false});
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

        this->gui->OpenChoiceDialog(dialog_message, user_choices);

        if (element_required && user_choices[2].second)
        {
            // User has chosen to delete the element's parent

            // Set the parent as the element to delete.
            this->element_to_delete = this->element_to_delete->GetParent();
            
            return this->Execute();
        }
        else if (user_choices[1].second) 
        {
            // User has chosen to cancel command

            return false;
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

bool DeleteElementCommand::ElementRequired()
{
    if (!(this->element_to_delete->GetRequired() == "1" || this->element_to_delete->GetRequired() == "+" || this->element_to_delete->GetRequired() == "*"))
    {
        return false;
    }
    else
    {
        // Go through each child 
        sdf::ElementPtr child_element_ptr = this->element_to_delete->GetParent()->GetFirstElement();
        while (child_element_ptr)
        {
            if ((child_element_ptr->GetName() == this->element_to_delete->GetName()) && (child_element_ptr.get() != this->element_to_delete.get()))
            {
                // Although the element_to_delete is required, there is child of the same type. Thus, element_to_delete is not strictly required
                return false;
            }
            child_element_ptr = child_element_ptr->GetNextElement("");
        }
        return true;
    }
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