#include "commands/GenericCommand.h"

GenericCommand::GenericCommand(std::function<void()> function_handle)
    : function_handle(function_handle)
{
}

bool GenericCommand::Execute()
{
    this->function_handle();
    return true;
}

bool GenericCommand::ExecuteUndo()
{
    return false;
}

bool GenericCommand::ExecuteRedo()
{
    return false;
}

bool GenericCommand::IsUndoable()
{
    return false;
}

bool GenericCommand::IsRedoable()
{
    return false;
}

bool GenericCommand::IsThreaded()
{
    return false;
}

bool GenericCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}
