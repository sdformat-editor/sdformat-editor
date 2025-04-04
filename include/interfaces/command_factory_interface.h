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

#ifndef COMMAND_FACTORY_INTERFACE_HH_
#define COMMAND_FACTORY_INTERFACE_HH_

// SDFormat dependencies
#include <sdf/sdf.hh>
#include <memory>
#include "interfaces/command_interface.h"
#include "interfaces/gui_interface.h"
#include "interfaces/sdformat_parser_interface.h"
#include "interfaces/model_viewer_interface.h"
#include <stack>

// Predeclare GUII to avoid circular dependencies
class GUII;

/// \brief Interface for the SDFormat Editor's Command Factory
class CommandFactoryI
{
  /// \callgraph
  /// \brief Initialization of the Command Factory. Should be wrapped in the constructor of the implementation.
  /// \param[in] gui A pointer to the GUI object
  /// \param[in] sdformat_parser A pointer to the sdformat parser object
  /// \param[in] model_viewer A pointer to the model viewer object
  private: virtual void Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, 
                                    std::shared_ptr<ModelViewerI> model_viewer) = 0;

  /// \brief Create an open file command
  /// \param[in] file_path Filepath of the model
  /// \return Unique pointer to a command interface 
  public: virtual std::unique_ptr<CommandI> MakeOpenFileCommand(std::string file_path) = 0;

  /// \brief Create a create file command
  /// \return Unique pointer to a command interface 
  public: virtual std::unique_ptr<CommandI> MakeCreateFileCommand() = 0;

  /// \brief Create a delete element command
  /// \param[in] element_to_delete The SDF element to delete
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeDeleteElementCommand(
      sdf::ElementPtr element_to_delete) = 0;

  /// \brief Create an add element command
  /// \param[in] parent_element The parent of the new SDF element
  /// \param[in] new_element The new SDF element
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeAddElementCommand(
    sdf::ElementPtr parent_element, sdf::ElementPtr new_element) = 0;

  /// \brief Create an undo command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeUndoCommand() = 0;
  
  /// \brief Create a redo command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeRedoCommand() = 0;

  /// \brief Create a save file command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeSaveFileCommand() = 0;

  /// \brief create a modify attribute command
  /// \param[in] attribute_to_modify Pointer to the attribute to modify
  /// \param[in] new_value The string value that will be given to the element
  /// \return a unique pointer to the commmand interface
  public: virtual std::unique_ptr<CommandI> MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, std::string new_value) = 0;

  
  /// \brief create a modify attribute command
  /// \param[in] attribute_to_modify Pointer to the attribute to modify
  /// \param[in] new_value The boolean value that will be given to the element
  /// \return a unique pointer to the commmand interface
  public: virtual std::unique_ptr<CommandI> MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, bool new_value) = 0;

  /// \brief create a modify element command
  /// \return a unique pointer to the commmand interface
  /// \param[in] element_to_modify Pointer to the element to modify
  /// \param[in] new_value The string value that will be given to the element
  public: virtual std::unique_ptr<CommandI> MakeModifyElementCommand(sdf::ElementPtr element_to_modify, std::string new_value) = 0;

  /// \brief create a modify element command
  /// \return a unique pointer to the commmand interface
  /// \param[in] element_to_modify Pointer to the element to modify
  /// \param[in] new_value The boolean value that will be given to the element
  public: virtual std::unique_ptr<CommandI> MakeModifyElementCommand(sdf::ElementPtr element_to_modify, bool new_value) = 0;

  /// \brief Create a render model command
  /// \param[in] render_collisions_in_model_viewer indicates if collision boxes should be rendered in the model viewer
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeRenderModelCommand(bool render_collisions_in_model_viewer) = 0;

  /// \brief Create a open model viewer model command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeOpenModelViewerCommand() = 0;

  /// \brief Create a close model viewer model command
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeCloseModelViewerCommand() = 0;

  /// \brief Pushes to the undo commands stack
  /// \param[in] command commandI object to push
  /// \param[in] new_change indicates if we are pushing a new change, and thus should clear the redo stack
  public: virtual void PushToUndoCommandsStack(std::unique_ptr<CommandI> command, const bool new_change = true) = 0;

  /// \brief Pushes to the redo commands stack
  /// \param[in] command commandI object to push
  public: virtual void PushToRedoCommandsStack(std::unique_ptr<CommandI> command) = 0;

  /// \brief Clears both the undo and redo stacks
  public: virtual void ClearUndoRedoStacks() = 0;
};

#endif // COMMAND_FACTORY_INTERFACE_HH_