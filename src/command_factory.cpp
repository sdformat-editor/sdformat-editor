#include "command_factory.h"
#include "commands/DeleteElementCommand.h"
#include "commands/GenericCommand.h"
#include "commands/OpenFileCommand.h"
#include "commands/ModifyAttributeCommand.h"
#include "commands/ModifyElementCommand.h"
#include "commands/AddElementCommand.h"

CommandFactory::CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser)
{
    this->Initialize(gui, sdformatParser);
}

void CommandFactory::Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
}

std::unique_ptr<CommandI> CommandFactory::MakeOpenFileCommand()
{
    return std::make_unique<OpenFileCommand>(this->gui, this->sdformatParser);
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
    return std::make_unique<ModifyAttributeCommand>(this->gui, this->sdformatParser, attribute_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeModifyElementCommand(sdf::ElementPtr element_to_modify, std::string new_value) {
    return std::make_unique<ModifyElementCommand>(this->gui, this->sdformatParser, element_to_modify, new_value);
}

std::unique_ptr<CommandI> CommandFactory::MakeAddElementCommand(sdf::ElementPtr parent_element, sdf::ElementPtr new_element)
{
    return std::make_unique<AddElementCommand>(this->gui, this->sdformatParser, parent_element, new_element);
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