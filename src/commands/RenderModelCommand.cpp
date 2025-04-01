#include "commands/RenderModelCommand.h"

RenderModelCommand::RenderModelCommand(std::shared_ptr<ModelViewerI> model_viewer)
{
    this->model_viewer = model_viewer;
}

bool RenderModelCommand::Execute()
{
    // implementation
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
