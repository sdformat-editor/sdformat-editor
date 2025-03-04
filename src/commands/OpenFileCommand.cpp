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


OpenFileCommand::OpenFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser) 
  : gui(gui), sdformatParser(sdformatParser)
{
  
}

bool OpenFileCommand::execute()
{

  std::string file_path = this->gui->OpenFileDialog();
  
  if (file_path == "")
  {
    return false;
  }
  
  bool success;

  std::unique_lock<std::mutex> lock_var = gui->lock_mutex();
  this->sdformatParser->Initialize(file_path, success);

  return success;

}

bool OpenFileCommand::threaded() 
{
  return true;
}

bool OpenFileCommand::undo()
{
  return false;
}

bool OpenFileCommand::redo()
{
  return false;
}
