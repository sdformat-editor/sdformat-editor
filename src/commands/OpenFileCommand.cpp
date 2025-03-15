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

#include "commands/OpenFileCommand.h"
#include "file_operations.h"


OpenFileCommand::OpenFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, std::string file_path) 
  : gui(gui), sdformatParser(sdformatParser), file_path(file_path)
{
  
}

bool OpenFileCommand::Execute()
{

  // If the parameter is blank, open the file dialog 
  if (file_path == "")
  {
    file_path = FileOperations::GetSoleInstance().OpenFileDialog();
  }

  // If the file path is still blank, then there was no file specified
  if (file_path == "")
  {
    return false;
  }
  
  bool success;

  std::unique_lock<std::mutex> lock_var = gui->LockMutex();
  this->sdformatParser->Initialize(file_path, success);

  if (success)
  {
    std::string directory = getenv("OLDPWD");
    std::string data_file = directory + std::string("/data/last_file_opened.txt");
    FILE *cache_file = fopen(data_file.c_str(), "w");
    const char *c = file_path.c_str();
    fwrite(c, sizeof(char), strlen(c), cache_file);
    fclose(cache_file);
  }

  return success;

}

bool OpenFileCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool OpenFileCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool OpenFileCommand::IsThreaded() 
{
  return true;
}

bool OpenFileCommand::IsUndoable()
{
  return false;
}

bool OpenFileCommand::IsRedoable()
{
  return false;
}

bool OpenFileCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return true;
}
