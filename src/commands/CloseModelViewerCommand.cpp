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

#include "commands/CloseModelViewerCommand.h"
#include <sys/stat.h>


CloseModelViewerCommand::CloseModelViewerCommand(std::shared_ptr<ModelViewerI> model_viewer) 
  : model_viewer(model_viewer)
{
  
}

bool CloseModelViewerCommand::Execute()
{
  {
      std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
      this->model_viewer->Quit();
  }

  auto isModelViewerRunning = [this]() -> bool {
      std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
      return this->model_viewer->IsRunning();
  };

  while (isModelViewerRunning());
  
  return true;

}

bool CloseModelViewerCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool CloseModelViewerCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool CloseModelViewerCommand::IsThreaded(bool& prevent_user_input) 
{
  prevent_user_input = false;
  return false;
}

bool CloseModelViewerCommand::IsUndoable()
{
  return false;
}

bool CloseModelViewerCommand::IsRedoable()
{
  return false;
}

bool CloseModelViewerCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}
