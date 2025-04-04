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

#include "commands/RenderModelCommand.h"

RenderModelCommand::RenderModelCommand(std::shared_ptr<ModelViewerI> model_viewer, std::shared_ptr<SDFormatParserI> sdformat_parser)
{
    this->model_viewer = model_viewer;
    this->sdformat_parser = sdformat_parser;
}

bool RenderModelCommand::Execute()
{
    std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());

    // Set a flag to remove reset the models in the model viewer
    this->model_viewer->ResetModels();

    // Get the model info from the SDFormat Parser (file locations, scale, pose)
    std::pair<std::vector<ModelViewerI::ModelInfo>, std::vector<ModelViewerI::PresetModelInfo>> models = this->sdformat_parser->GetModelsFromSDFTree();

    // Add both mesh models and preset (simple shape) models 
    for (ModelViewerI::ModelInfo model : models.first) 
    {
        this->model_viewer->AddModel(model);
    }
    for (ModelViewerI::PresetModelInfo model : models.second) 
    {
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

bool RenderModelCommand::IsThreaded(bool& prevent_user_input)
{
    prevent_user_input = false;
    return false;
}

bool RenderModelCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}
