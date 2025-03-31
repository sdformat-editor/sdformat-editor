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

#ifndef SDFORMAT_EDITOR_DELETE_SCENE_COMMAND_HH_
#define SDFORMAT_EDITOR_DELETE_SCENE_COMMAND_HH_


#include <interfaces/command_interface.h>


/// \brief Delete Scene command implementation of CommandI
class DeleteSceneCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for delete scene command objects.
  /// \param[in] 
  public: DeleteSceneCommand();

  /// \brief Implementation of interface method. 
  /// \returns True if model is successfully loaded in model viewer
  private: bool Execute() override;

  /// \brief Implementation of interface method. 
  /// \returns Always false
  private: bool ExecuteUndo() override;

  /// \brief Implementation of interface method. 
  /// \returns Always false
  private: bool ExecuteRedo() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool IsUndoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool IsRedoable() override;

  /// \brief Implementation of interface method.
  /// \returns Always true
  private: bool IsThreaded() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

};

#endif