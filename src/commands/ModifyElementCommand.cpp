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

#include "commands/ModifyElementCommand.h"

template class ModifyElementCommand<std::string>;
template class ModifyElementCommand<bool>;

template <typename T> ModifyElementCommand<T>::ModifyElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr element_to_modify, T new_value) : gui(gui), sdformatParser(sdformatParser), element_to_modify(element_to_modify), new_value(new_value) {}

template <typename T> bool ModifyElementCommand<T>::Execute()
{
    if (!element_to_modify->GetValue()->Get(old_value)) 
    {
    
        const std::string dialog_message_header = "Error";
        const std::string dialog_message_body = "Could not modify element value.";
        const std::string dialog_message_footer = "Element does not contain the old value.";

        std::vector<std::pair<std::string, bool>> user_choices;

        user_choices.push_back({"OK", false});

        GUII::DialogMessage dialog_message{dialog_message_header, dialog_message_body, dialog_message_footer};

        this->gui->OpenChoiceDialog(dialog_message, user_choices);

        return false;
    }
    else
    {
        SDFormatParserI::Mentions mentions;
    
        // Find mentions of the element to modify, excluding the element itself 
        if (auto name_attribute = this->element_to_modify->GetAttribute("name"))
        {
            mentions = this->sdformatParser->FindMentions(name_attribute->GetAsString(), this->element_to_modify);
        }
    
        // If this element has mentions anywhere else, display a choice dialog to the user
        if (!mentions.attributes.empty() || !mentions.elements.empty())
        {
    
            const std::string dialog_message_header = "Warning";
            const std::string dialog_message_footer = "You are modifying an element that is mentioned elsewhere.";
            std::string dialog_message_body;
    
            std::vector<std::pair<std::string, bool>> user_choices;
    
            user_choices.push_back({"Proceed", false});
            user_choices.push_back({"Cancel", false});
    
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
    
            if (user_choices[1].second) 
            {
                // User has chosen to cancel command
                return false;
            }
        }
        
        if (element_to_modify->Set(new_value))
        {
            this->is_currently_undoable = true;
            this->is_currently_redoable = false;

            return true;
        }
        
        return false;
    }
}

template <typename T> bool ModifyElementCommand<T>::ExecuteUndo()
{
    if (element_to_modify->Set(old_value))
    {
        this->is_currently_undoable = false;
        this->is_currently_redoable = true;
        
        return true;
    } 
    return false;
}

template <typename T> bool ModifyElementCommand<T>::ExecuteRedo()
{
    if (element_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;
    
        return true;
    }
    return false;
}

template <typename T> bool ModifyElementCommand<T>::IsUndoable()
{
    return this->is_currently_undoable;
}

template <typename T> bool ModifyElementCommand<T>::IsRedoable()
{
    return this->is_currently_redoable;
}

template <typename T> bool ModifyElementCommand<T>::IsThreaded()
{
    // Stub implementation
    return false;
}

template <typename T> bool ModifyElementCommand<T>::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}