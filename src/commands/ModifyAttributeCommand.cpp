#include "commands/ModifyAttributeCommand.h"

template class ModifyAttributeCommand<bool>;
template class ModifyAttributeCommand<std::string>;

template <typename T> ModifyAttributeCommand<T>::ModifyAttributeCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ParamPtr attribute_to_modify, T new_value) : gui(gui), sdformatParser(sdformatParser), attribute_to_modify(attribute_to_modify), new_value(new_value) {}

template <typename T> bool ModifyAttributeCommand<T>::Execute()
{
    if (!attribute_to_modify->Get(old_value)) 
    {
    
        const std::string dialog_message_header = "Error";
        const std::string dialog_message_body = "Could not modify attribute value.";
        const std::string dialog_message_footer = "Attribute does not contain the old value.";

        std::vector<std::pair<std::string, bool>> user_choices;

        user_choices.push_back({"OK", false});

        GUII::DialogMessage dialog_message{dialog_message_header, dialog_message_body, dialog_message_footer};

        this->gui->OpenChoiceDialog(dialog_message, user_choices);

        return false;
    }
    else
    {   
        if (this->attribute_to_modify->GetKey() == "name")
        {   
            // If this is a name attribute, warn the user if it is mentioned anywhere
            SDFormatParserI::Mentions mentions = this->sdformatParser->FindMentions(attribute_to_modify->GetAsString(), this->attribute_to_modify->GetParentElement());

            // If this element has mentions anywhere else, display a choice dialog to the user
            if (!mentions.attributes.empty() || !mentions.elements.empty())
            {
        
                const std::string dialog_message_header = "Warning";
                const std::string dialog_message_footer = "You are modifying a name attribute that is mentioned elsewhere.";
                std::string dialog_message_body;
        
                std::vector<std::pair<std::string, bool>> user_choices;
        
                user_choices.push_back({"Proceed", false});
                user_choices.push_back({"Cancel", false});
        
                if (!mentions.elements.empty())
                {
                    dialog_message_body += "The following elements mention this name attribute: \n";
                    for (const auto &element_mention : mentions.elements)
                    {
                        dialog_message_body += "- " + this->sdformatParser->GetSDFTreePathToElement(element_mention) + "\n";
                    }
                }
        
                if (!mentions.attributes.empty())
                {
                    dialog_message_body += "The following attributes mention this name attribute: \n";
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
        }
        
        if (attribute_to_modify->Set(new_value))
        {
            this->is_currently_undoable = true;
            this->is_currently_redoable = false;

            return true;
        }
        return false;
    }
}

template <typename T> bool ModifyAttributeCommand<T>::ExecuteUndo()
{
    if (attribute_to_modify->Set(old_value))
    {
        this->is_currently_undoable = false;
        this->is_currently_redoable = true;

        return true;
    }
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::ExecuteRedo()
{
    if (attribute_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;
    
        return true;
    }
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::IsUndoable()
{
    return this->is_currently_undoable;
}

template <typename T> bool ModifyAttributeCommand<T>::IsRedoable()
{
    return this->is_currently_redoable;
}

template <typename T> bool ModifyAttributeCommand<T>::IsThreaded()
{
    // Stub implementation
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}