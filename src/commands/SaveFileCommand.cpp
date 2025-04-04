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


SaveFileCommand::SaveFileCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser) 
  : gui(gui), sdformatParser(sdformatParser)
{
  
}

bool SaveFileCommand::Execute()
{

  if (this->sdformatParser->GetSDFElement())
  {
    const std::string dialog_message_header = "Warning";
    const std::string dialog_message_body = "Saving will overwrite existing file contents.";
    const std::string dialog_message_footer = "Are you sure you want to continue?";
  
    GUII::DialogMessage dialog_message{dialog_message_header, dialog_message_body, dialog_message_footer};
  
    std::vector<std::pair<std::string, bool>> user_choices;
  
    user_choices.push_back({"Proceed", false});
    user_choices.push_back({"Cancel", false});
  
    this->gui->OpenChoiceDialog(dialog_message, user_choices);
  
    if (user_choices[0].second)
    {
      // User has chosen to proceed
  
      bool file_saved = false;
    
      // Attempt to save the XML 
      // TODO: Add functionality to conserve existing comments, maintian relative file paths, and maintain the order of unmodified elements 
      {
        std::unique_lock<std::mutex> lock_var = gui->LockMutex();
  
        std::string x = this->sdformatParser->GetSDFElement()->ToString();
        file_saved = FileOperations::GetSoleInstance().WriteFile(x);
      }
  
      const std::string dialog_message_header = "Info";
      const std::string dialog_message_footer = file_saved ? "File saved." : "Could not save file. Are you sure this file exists and is writeable?";
  
      GUII::DialogMessage dialog_message{dialog_message_header, "", dialog_message_footer};
  
      std::vector<std::pair<std::string, bool>> user_choices = {{"Ok", false}};
  
      this->gui->OpenChoiceDialog(dialog_message, user_choices);
  
      return file_saved;
    }
    else
    {
      return false;
    }
  }
  else
  {
    const std::string dialog_message_header = "Info";
    const std::string dialog_message_footer = "Cannot execute, there is no SDF tree loaded";
  
    GUII::DialogMessage dialog_message{dialog_message_header, "", dialog_message_footer};
  
    std::vector<std::pair<std::string, bool>> user_choices;
  
    user_choices.push_back({"Ok", false});
  
    this->gui->OpenChoiceDialog(dialog_message, user_choices);

    return false;
  }
}

bool SaveFileCommand::ExecuteUndo()
{
  return this->IsUndoable();
}

bool SaveFileCommand::ExecuteRedo()
{
  return this->IsRedoable();
}

bool SaveFileCommand::IsThreaded(bool& prevent_user_input) 
{
  prevent_user_input = false;
  return false;
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
