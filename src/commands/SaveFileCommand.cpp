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

#include "commands/SaveFileCommand.h"
#include "file_operations.h"


SaveFileCommand::SaveFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, bool force_save_as_not_save) 
  : gui(gui), sdformatParser(sdformatParser)
{
  
}

bool SaveFileCommand::Execute()
{
  
  std::unique_lock<std::mutex> lock_var = gui->LockMutex();

  
  this->sdformatParser->GetSDFElement()->ToString();
  std::string x = this->sdformatParser->GetSDFElement()->ToString();
  FileOperations::GetSoleInstance().WriteFile(x);

  return true;
}

bool SaveFileCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool SaveFileCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool SaveFileCommand::IsThreaded() 
{
  return true;
}

bool SaveFileCommand::IsUndoable()
{
  return false;
}

bool SaveFileCommand::IsRedoable()
{
  return false;
}

bool SaveFileCommand::ChangesProgramStateIrreversibly()
{
  return false;
}
