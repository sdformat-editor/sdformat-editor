#include "commands/RenderModelCommand.h"

RenderModelCommand::RenderModelCommand(std::shared_ptr<ModelViewerI> model_viewer, std::shared_ptr<SDFormatParserI> sdformat_parser)
{
    this->model_viewer = model_viewer;
    this->sdformat_parser = sdformat_parser;
}

bool RenderModelCommand::Execute()
{
    // TODO: add functionallity to remove all models before rerendering
    std::vector<ModelViewerI::ModelInfo> models = this->sdformat_parser->GetModelsFromSDFTree();
    for (ModelViewerI::ModelInfo model : models) {
        this->model_viewer->AddModel(model);
    }
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
