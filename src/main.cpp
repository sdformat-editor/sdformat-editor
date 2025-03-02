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

#include "gui.h"
#include "sdformat_parser.h"
#include "interfaces/command_interface.h"
#include <thread>

// Main code
int main(int, char **)
{
    bool gui_initalization_successful;

    GUII* gui = new GUI("SDFormat Editor", gui_initalization_successful);

    // Exit the program if the GUI cannot initalize
    // This may happen if there is no active display
    if (!gui_initalization_successful) return 1;

    // The sdformatParser will be null until the user opens a file
    SDFormatParserI* sdformatParser = nullptr;

    // Stacks for undo/redo functionality
    // TODO: Actually do something with these stacks
    std::vector<std::unique_ptr<CommandI>> undo_commands_stack;
    std::vector<std::unique_ptr<CommandI>> redo_commands_stack;

    // gui->ShouldClose() will become true when the window is closed by the user
    while (!gui->ShouldClose())
    {
        // Poll the GUI for user input
        // Update will return nullptr if the user does nothing
        std::unique_ptr<CommandI> user_command(gui->Update(sdformatParser));

        if (user_command)
        {
            // Some commands require a different thread (ex. OpenFileCommand)
            if (user_command->threaded())
            {
                // Make a thread for executing this command
                std::thread command_thread([&user_command, &undo_commands_stack, gui]() {
                
                    // (zaid) I don't forsee there being a time where we really need to take user input
                    // while an external thread is doing some operation (ex. opening a file). To make things
                    // simpler for now, the gui will not create any more commands while an external thread is running.
                    gui->prevent_input_flag = true;   

                    if (user_command->execute())
                    {
                        // Add the command to the undo stack if it executes successfully
                        undo_commands_stack.push_back(std::move(user_command));
                    }

                    // Allow the GUI to take user commands
                    gui->prevent_input_flag = false;   
                });             

                // Detach this thread such that it will be automatically destroyed when finished
                command_thread.detach();
            }
            else if (user_command->execute())
            {
                undo_commands_stack.push_back(std::move(user_command));
            }
        }
    }

    // Call the destructor of gui
    delete gui;

    return 0;
}
