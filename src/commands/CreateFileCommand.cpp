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

#include "commands/CreateFileCommand.h"
#include "file_operations.h"
#include <sys/stat.h>


CreateFileCommand::CreateFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser) 
  :gui(gui), sdformatParser(sdformatParser)
{
  
}

bool CreateFileCommand::Execute()
{
  // Prevent user input in the gui while they are opening a file

  // If the parameter is blank, open the file dialog 
  std::string file_path = FileOperations::GetSoleInstance().OpenDirectoryDialog();
  
  // If the file path is blank, then there was no directory specified
  if (file_path == "")
  {
    return false;
  }

  // Create a new .sdf within the specified directory
  std::string sdf_file = file_path + "/model.sdf";

  FileOperations::GetSoleInstance().SetActiveFilePath(sdf_file);

  FILE *new_model = fopen(sdf_file.c_str(), "w");
  
  const char *sdf_contents = "<?xml version='1.0'?>\n<sdf version='1.11'>\n</sdf>\n";
  fwrite(sdf_contents, sizeof(char), strlen(sdf_contents), new_model);

  fclose(new_model);
  
  bool success;

  this->sdformatParser->Initialize(sdf_file, success);

  // Save the path to an external file
  if (success)
  {
    std::string data_dir = std::string(getenv("HOME")) + "/.local/share/sdformat_editor/";
    struct stat info;
    // Check if data directory exists
    if (stat(data_dir.c_str(), &info) == 0)
    {
      std::string data_file = data_dir + "last_file_opened.txt";
      FILE *cache_file = fopen(data_file.c_str(), "w");
      const char *c = sdf_file.c_str();
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

bool CreateFileCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool CreateFileCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool CreateFileCommand::IsThreaded(bool& prevent_user_input) 
{
  prevent_user_input = true;
  return true;
}

bool CreateFileCommand::IsUndoable()
{
  return false;
}

bool CreateFileCommand::IsRedoable()
{
  return false;
}

bool CreateFileCommand::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return true;
}
