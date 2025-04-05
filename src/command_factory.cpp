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

#include "command_factory.h"

CommandFactory::CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, 
                                    std::shared_ptr<ModelViewerI> model_viewer)
{
    this->Initialize(gui, sdformatParser, model_viewer);
}

void CommandFactory::Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, 
                            std::shared_ptr<ModelViewerI> model_viewer)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
    this->model_viewer = model_viewer;
}

std::unique_ptr<CommandI> CommandFactory::MakeOpenFileCommand(std::string file_path)
{
    return std::make_unique<OpenFileCommand>(this->gui, this->sdformatParser, file_path);
}

std::unique_ptr<CommandI> CommandFactory::MakeRenderModelCommand(bool render_collisions_in_model_viewer)
{
    return std::make_unique<RenderModelCommand>(this->model_viewer, this->sdformatParser, render_collisions_in_model_viewer);
}

std::unique_ptr<CommandI> CommandFactory::MakeOpenModelViewerCommand()
{
    return std::make_unique<OpenModelViewerCommand>(this->model_viewer);
}

std::unique_ptr<CommandI> CommandFactory::MakeCreateFileCommand()
{
    return std::make_unique<CreateFileCommand>(this->gui, this->sdformatParser);
}

std::unique_ptr<CommandI> CommandFactory::MakeUndoCommand()
{
    return std::make_unique<GenericCommand>([this]() { this->PopFromUndoCommandsStack();});
}

std::unique_ptr<CommandI> CommandFactory::MakeRedoCommand()
{
    return std::make_unique<GenericCommand>([this]() { this->PopFromRedoCommandsStack();});
}

std::unique_ptr<CommandI> CommandFactory::MakeDeleteElementCommand(sdf::ElementPtr element_to_delete)
{
    return std::make_unique<DeleteElementCommand>(this->gui, this->sdformatParser, element_to_delete);
}

std::unique_ptr<CommandI> CommandFactory::MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, std::string new_value) {
    return std::make_unique<ModifyAttributeCommand<std::string>>(this->gui, this->sdformatParser, attribute_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, bool new_value) {
    return std::make_unique<ModifyAttributeCommand<bool>>(this->gui, this->sdformatParser, attribute_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeModifyElementCommand(sdf::ElementPtr element_to_modify, std::string new_value) {
    return std::make_unique<ModifyElementCommand<std::string>>(this->gui, this->sdformatParser, element_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeModifyElementCommand(sdf::ElementPtr element_to_modify, bool new_value) {
    return std::make_unique<ModifyElementCommand<bool>>(this->gui, this->sdformatParser, element_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeAddElementCommand(sdf::ElementPtr parent_element, sdf::ElementPtr new_element)
{
    return std::make_unique<AddElementCommand>(this->gui, this->sdformatParser, parent_element, new_element);
}


std::unique_ptr<CommandI> CommandFactory::MakeCloseModelViewerCommand()
{
    return std::make_unique<CloseModelViewerCommand>(this->model_viewer);
}

void CommandFactory::PushToUndoCommandsStack(std::unique_ptr<CommandI> command, const bool new_change)
{
    if (new_change)
    {
        this->ClearStack(this->redo_commands_stack);
    }
    this->undo_commands_stack.emplace(std::move(command));
}

void CommandFactory::PushToRedoCommandsStack(std::unique_ptr<CommandI> command)
{
    this->redo_commands_stack.emplace(std::move(command));
}

void CommandFactory::PopFromUndoCommandsStack()
{
    if (!(this->undo_commands_stack.empty()))
    {
        this->undo_commands_stack.top()->ExecuteUndo();
        if (this->undo_commands_stack.top()->IsRedoable())
        {
            this->PushToRedoCommandsStack(std::move(this->undo_commands_stack.top()));
        }
        this->undo_commands_stack.pop();
    }
}

void CommandFactory::PopFromRedoCommandsStack()
{
    if (!(this->redo_commands_stack.empty()))
    {
        this->redo_commands_stack.top()->ExecuteRedo();
        if (this->redo_commands_stack.top()->IsUndoable())
        {
            // Set the new_change parameter to false so as to not clear the redo stack 
            this->PushToUndoCommandsStack(std::move(this->redo_commands_stack.top()), false);
        }
        this->redo_commands_stack.pop();
    }
}

void CommandFactory::ClearUndoRedoStacks()
{
    this->ClearStack(this->undo_commands_stack);
    this->ClearStack(this->redo_commands_stack);
}

void CommandFactory::ClearStack(std::stack<std::unique_ptr<CommandI>>& stack)
{
    while (!stack.empty())
    {
        stack.pop();
    }
}

std::unique_ptr<CommandI> CommandFactory::MakeSaveFileCommand()
{
    return std::make_unique<SaveFileCommand>(this->gui, this->sdformatParser);
}