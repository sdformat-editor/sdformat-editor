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
#include <sys/stat.h>


OpenFileCommand::OpenFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, std::string file_path) 
  : file_path(file_path), gui(gui), sdformatParser(sdformatParser)
{
  
}

bool OpenFileCommand::Execute()
{

  // If the parameter is blank, open the file dialog 
  if (file_path == "")
  {
    file_path = FileOperations::GetSoleInstance().OpenFileDialog();
  } 
  else 
  {
    FILE *test_file = fopen(file_path.c_str(), "r");
    if (test_file == NULL) {
      return false;
    }
    fclose(test_file);
    FileOperations::GetSoleInstance().SetActiveFilePath(file_path);
  }

  // If the file path is still blank, then there was no file specified
  if (file_path == "")
  {
    return false;
  }
  
  bool success;

  std::unique_lock<std::mutex> lock_var = gui->LockMutex();
  this->sdformatParser->Initialize(file_path, success);

  //Save the file path to an external file
  if (success)
  {
    std::string data_dir = std::string(getenv("HOME")) + "/.local/share/sdformat_editor/";
    struct stat info;
    // Check if data directory exists
    if (stat(data_dir.c_str(), &info) == 0)
    {
      std::string data_file = data_dir + "last_file_opened.txt";
      FILE *cache_file = fopen(data_file.c_str(), "w");
      const char *c = file_path.c_str();
      fwrite(c, sizeof(char), strlen(c), cache_file);
      fclose(cache_file);
    } 
    else 
    {
      std::cerr << "Failed to locate directory!" << std::endl;
    }
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
