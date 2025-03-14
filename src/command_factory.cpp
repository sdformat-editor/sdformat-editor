#include "command_factory.h"
#include "commands/DeleteElementCommand.h"
#include "commands/GenericCommand.h"
#include "commands/OpenFileCommand.h"

CommandFactory::CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser)
{
    this->Initialize(gui, sdformatParser);
}

void CommandFactory::Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser)
{
    this->gui = gui;
    this->sdformatParser = sdformatParser;
}

std::unique_ptr<CommandI> CommandFactory::MakeOpenFileCommand(std::string file_path)
{
    return std::make_unique<OpenFileCommand>(this->gui, this->sdformatParser, file_path);
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

std::unique_ptr<CommandI> CommandFactory::MakeSaveFileCommand(bool force_save_as_not_save)
{
    return std::make_unique<SaveFileCommand>(this->gui, this->sdformatParser, force_save_as_not_save);
}