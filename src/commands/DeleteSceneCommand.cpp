#include "commands/DeleteSceneCommand.h"

DeleteSceneCommand::DeleteSceneCommand(std::shared_ptr<ModelViewerI> model_viewer)
{
    this->model_viewer = model_viewer;
}

bool DeleteSceneCommand::Execute()
{
    this->model_viewer->Quit();
    while (this->model_viewer->IsRunning())
    {

    }
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
