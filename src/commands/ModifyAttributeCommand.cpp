#include "commands/ModifyAttributeCommand.h"

ModifyAttributeCommand::ModifyAttributeCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ParamPtr attribute_to_modify, std::string new_value) : gui(gui), sdformatParser(sdformatParser), attribute_to_modify(attribute_to_modify), new_value(new_value) {}

bool ModifyAttributeCommand::Execute()
{
    old_value = attribute_to_modify->GetAsString();
    attribute_to_modify->SetFromString(new_value);
    return true;
}

bool ModifyAttributeCommand::ExecuteUndo()
{
    attribute_to_modify->SetFromString(old_value);
    return true;
}

bool ModifyAttributeCommand::ExecuteRedo()
{
    attribute_to_modify->SetFromString(new_value);
    return true;
}

bool ModifyAttributeCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool ModifyAttributeCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool ModifyAttributeCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool ModifyAttributeCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}