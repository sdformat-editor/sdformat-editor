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

FileEditorRunner::FileEditorRunner(bool data_dir_created)
{
    // The sdformatParser will be null until the user opens a file
    this->sdformatParser = std::make_shared<SDFormatParser>();

    this->model_viewer = std::make_shared<ModelViewer>();

    this->gui = std::make_shared<GUI>("SDFormat Editor", this->sdformatParser, this->gui_initalization_successful);
    
    // Run the model viewer as a seperate thread (make this a threadded command!)
    // std::thread model_viewer_thread(&FileEditorRunner::RunModelViewerThread, this);
    // model_viewer_thread.detach();

    this->command_factory = std::make_shared<CommandFactory>(this->gui, this->sdformatParser, this->model_viewer);

    // grab the previous file that was opened
    if (data_dir_created) {
        std::string data_file = std::string(getenv("HOME")) + "/.local/share/sdformat_editor/last_file_opened.txt";
        FILE *cache_file = fopen(data_file.c_str(), "r");
        char buffer[100];
        size_t bytesRead;
        if (cache_file != NULL)
        {
            std::unique_ptr<CommandI> command;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer) - 1, cache_file)) > 0){
        
                // Null-terminate the buffer
                buffer[bytesRead] = '\0'; 
                
                // Open the sdf file from last execution
                std::string previous_file_path = std::string(buffer);
                command = this->command_factory->MakeOpenFileCommand(previous_file_path);
            };
            fclose(cache_file);
            if (command != NULL)
            {
                std::thread command_thread([command = std::move(command), this]() mutable {
                    
                    bool prevent_user_input = false;
                    
                    // Check if this command requires that user input be prevented
                    (void)command->IsThreaded(prevent_user_input);

                    this->gui->SetPreventInputFlag(prevent_user_input);   

                    command->Execute();

                    // Allow the GUI to take user commands
                    this->gui->SetPreventInputFlag(false); 
                });
                // Detach the thread such that it runs in the background
                command_thread.detach();
            }
        };
    }
}

int FileEditorRunner::RunProgram()
{
    // Exit the program if the GUI cannot initalize
    // This may happen if there is no active display
    if (!this->gui_initalization_successful) return 1;

    // gui->ShouldClose() will become true when the window is closed by the user
    while (!this->gui->ShouldClose())
    {
        // Poll the GUI for user input
        // Update will return nullptr if the user does nothing
        std::unique_ptr<CommandI> user_command = this->gui->Update(this->command_factory);

        if (user_command)
        {
            bool prevent_user_input = false;
            // Some commands require a different thread (ex. OpenFileCommand)
            if (user_command->IsThreaded(prevent_user_input))
            {
                // Make a thread for executing this command
                std::thread command_thread([user_command = std::move(user_command), prevent_user_input, this]() mutable {
                    
                    this->gui->SetPreventInputFlag(prevent_user_input);   

                    if (user_command->Execute())
                    {
                        if (user_command->ChangesProgramStateIrreversibly())
                        {
                            this->command_factory->ClearUndoRedoStacks();
                        }
                        if (user_command->IsUndoable())
                        {
                            // Add the command to the undo stack if it executes successfully
                            this->command_factory->PushToUndoCommandsStack(std::move(user_command));
                        }
                    }

                    // Allow the GUI to take user commands
                    this->gui->SetPreventInputFlag(false); 
                    
                });             
                
                // Detach the thread such that it runs in the background
                command_thread.detach();
            }
            else if (user_command->Execute())
            {
                if (user_command->ChangesProgramStateIrreversibly())
                {
                    this->command_factory->ClearUndoRedoStacks();
                }
                if (user_command->IsUndoable())
                {
                    // Add the command to the undo stack if it executes successfully
                    this->command_factory->PushToUndoCommandsStack(std::move(user_command));
                }
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
        this->model_viewer->Quit();
    }

    auto isModelViewerRunning = [this]() -> bool {
        std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
        return this->model_viewer->IsRunning();
    };

    while (isModelViewerRunning());

    return 0;
}