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

#ifndef SDFORMAT_EDITOR_GENERIC_COMMAND_HH_
#define SDFORMAT_EDITOR_GENERIC_COMMAND_HH_


#include <interfaces/command_interface.h>

#include <functional>

/// \brief Executes and pops the top commandI object from a stack
class GenericCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for open model command objects.
  /// \param[in] function_pointer unction pointer to hold the command function
  public: GenericCommand(std::function<void()> function_handle);

  /// \brief Implementation of interface method. 
  /// \returns Always true. RExecutes and pops the top commandI object from a stack
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
  /// \returns Always false
  private: bool IsThreaded() override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

  /// \brief Function pointer to hold the command function
  std::function<void()> function_handle;

};

#endif