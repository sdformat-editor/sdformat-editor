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

#include "commands/OpenModelViewerCommand.h"
#include "file_operations.h"
#include <sys/stat.h>


OpenModelViewerCommand::OpenModelViewerCommand(std::shared_ptr<ModelViewerI> model_viewer) 
  : model_viewer(model_viewer)
{
  
}

bool OpenModelViewerCommand::Execute()
{
  {
      std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
      this->model_viewer->Initialize();
  }

  auto isModelViewerRunning = [this]() -> bool {
      std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());
      return this->model_viewer->IsRunning();
  };

  while (isModelViewerRunning())
  {
      {
          std::lock_guard<std::mutex> lock(this->model_viewer->GetMutex());

          // Call the RenderFrame method
          this->model_viewer->RenderFrame();
      }

      // Sleep to achieve ~60Hz frame rate for the model viewer
      std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
  }

  return true;

}

bool OpenModelViewerCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool OpenModelViewerCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool OpenModelViewerCommand::IsThreaded(bool& prevent_user_input) 
{
  prevent_user_input = false;
  return true;
}

bool OpenModelViewerCommand::IsUndoable()
{
  return false;
}

bool OpenModelViewerCommand::IsRedoable()
{
  return false;
}

bool OpenModelViewerCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return true;
}
