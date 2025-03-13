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

bool DeleteElementCommand::execute()
{

    SDFormatParserI::Mentions mentions;

    // Can determine if the element is required with this->element_to_delete->GetRequired() 
    if (auto name_attribute = this->element_to_delete->GetAttribute("name"))
    {
        mentions = this->sdformatParser->FindMentions(name_attribute->GetAsString());
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
    }

    // TODO (zaid): finish the OpenYesNoDialog implementation in the GUI and pass in the paramters that are initalized above
    // Then look into the "undo/redo" functionality of this command in case the user says yes
    // I gotta go to class

    // Everything below is based on the "delete instantly" implemenation

    // Store the parent
    this->element_to_deletes_parent = element_to_delete->GetParent();
    
    // Remove the element to delete from it's parent
    element_to_delete->RemoveFromParent();
    return true;
}

bool DeleteElementCommand::execute_undo()
{
    element_to_deletes_parent->InsertElement(element_to_delete);
    return true;
}

bool DeleteElementCommand::execute_redo()
{
    return execute();
}

bool DeleteElementCommand::is_undoable()
{
    // Stub implementation
    return true;
}

bool DeleteElementCommand::is_redoable()
{
    // Stub implementation
    return true;
}

bool DeleteElementCommand::is_threaded()
{
    // Stub implementation
    return false;
}