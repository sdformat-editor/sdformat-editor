#include "commands/ModifyElementCommand.h"

ModifyElementCommand::ModifyElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr element_to_modify, std::string new_value) : gui(gui), sdformatParser(sdformatParser), element_to_modify(element_to_modify), new_value(new_value) {}

bool ModifyElementCommand::Execute()
{
    old_value = element_to_modify->GetValue()->GetAsString();
    if (element_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;

        return true;
    }
    
    return false;
}

bool ModifyElementCommand::ExecuteUndo()
{
    if (element_to_modify->Set(old_value))
    {
        this->is_currently_undoable = false;
        this->is_currently_redoable = true;
        
        return true;
    } 
    return false;
}

bool ModifyElementCommand::ExecuteRedo()
{
    if (element_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;
    
        return true;
    }
    return false;
}

bool ModifyElementCommand::IsUndoable()
{
    return this->is_currently_undoable;
}

bool ModifyElementCommand::IsRedoable()
{
    return this->is_currently_redoable;
}

bool ModifyElementCommand::IsThreaded()
{
    // Stub implementation
    return false;
}

bool ModifyElementCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}