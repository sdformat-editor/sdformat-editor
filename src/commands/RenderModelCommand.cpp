#include "commands/RenderModelCommand.h"

RenderModelCommand::RenderModelCommand(std::shared_ptr<ModelViewerI> model_viewer)
{
    this->model_viewer = model_viewer;
}

bool RenderModelCommand::Execute()
{
    if (this->model_viewer->IsRunning())
    {
        this->model_viewer->Quit();
        while (this->model_viewer->IsRunning())
        {

        }
    }
    
    std::thread model_viewer_thread(&ModelViewerI::RunModelViewerThread, model_viewer);
    model_viewer_thread.detach();
    
    return true;
}

bool RenderModelCommand::ExecuteUndo()
{
    return false;
}

bool RenderModelCommand::ExecuteRedo()
{
    return false;
}

bool RenderModelCommand::IsUndoable()
{
    return false;
}

bool RenderModelCommand::IsRedoable()
{
    return false;
}

bool RenderModelCommand::IsThreaded()
{
    return false;
}

bool RenderModelCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}
