#include "commands/DeleteSceneCommand.h"

DeleteSceneCommand::DeleteSceneCommand()
{

}

bool DeleteSceneCommand::Execute()
{
    // implementation
    return true;
}

bool DeleteSceneCommand::ExecuteUndo()
{
    return false;
}

bool DeleteSceneCommand::ExecuteRedo()
{
    return false;
}

bool DeleteSceneCommand::IsUndoable()
{
    return false;
}

bool DeleteSceneCommand::IsRedoable()
{
    return false;
}

bool DeleteSceneCommand::IsThreaded()
{
    return false;
}

bool DeleteSceneCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}
