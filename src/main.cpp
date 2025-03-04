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

#include "file_editor_runner.h"
#include <thread>

FileEditorRunner::FileEditorRunner()
{
    
    // The sdformatParser will be null until the user opens a file
    this->sdformatParser = std::make_shared<SDFormatParser>();

    this->gui = std::make_shared<GUI>("SDFormat Editor", this->sdformatParser, this->gui_initalization_successful);

    // TODO: Actually do something with the stacks
    
}

int FileEditorRunner::run_program()
{
    // Exit the program if the GUI cannot initalize
    // This may happen if there is no active display
    if (!this->gui_initalization_successful) return 1;

    // gui->ShouldClose() will become true when the window is closed by the user
    while (!this->gui->ShouldClose())
    {
        // Poll the GUI for user input
        // Update will return nullptr if the user does nothing
        std::unique_ptr<CommandI> user_command = this->gui->Update();

        if (user_command)
        {
            // Some commands require a different thread (ex. OpenFileCommand)
            if (user_command->threaded())
            {
                // Make a thread for executing this command
                std::thread command_thread([user_command = std::move(user_command), this]() mutable {
                
                    // (zaid) I don't forsee there being a time where we really need to take user input
                    // while an external thread is doing some operation (ex. opening a file). To make things
                    // simpler for now, the gui will not create any more commands while an external thread is running.
                    this->gui->prevent_input_flag = true;   

                    if (user_command->execute())
                    {
                        // Add the command to the undo stack if it executes successfully
                        this->undo_commands_stack.push_back(std::move(user_command));
                    }

                    // Allow the GUI to take user commands
                    this->gui->prevent_input_flag = false; 
                    
                });             
                
                // Detach the thread such that it runs in the background
                command_thread.detach();
            }
            else if (user_command->execute())
            {
                this->undo_commands_stack.push_back(std::move(user_command));
            }
        }
    }

    return 0;
}

// Entry point
int main(int, char **)
{
    // Create instance of main program
    FileEditorRunner program;

    // Status of termination
    int status;

    status = program.run_program();

    return status;

}
