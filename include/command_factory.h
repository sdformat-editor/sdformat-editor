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

#ifndef COMMAND_FACTORY_HH_
#define COMMAND_FACTORY_HH_

// SDFormat dependencies
#include <sdf/sdf.hh>
#include "interfaces/command_factory_interface.h"

// Commands
#include "commands/DeleteElementCommand.h"
#include "commands/OpenFileCommand.h"
#include "commands/SaveFileCommand.h"
#include "commands/DeleteElementCommand.h"
#include "commands/GenericCommand.h"
#include "commands/OpenFileCommand.h"
#include "commands/CreateFileCommand.h"
#include "commands/ModifyAttributeCommand.h"
#include "commands/ModifyElementCommand.h"
#include "commands/AddElementCommand.h"
#include "commands/RenderModelCommand.h"
#include "commands/OpenModelViewerCommand.h"
#include "commands/CloseModelViewerCommand.h"

/// \brief Implementation of CommandFactoryI
class CommandFactory : public CommandFactoryI
{

    /// \brief Constructor that wraps the Initialize method
    /// \param[in] gui A pointer to the GUI object
    /// \param[in] sdformat_parser A pointer to the sdformat parser object
    /// \param[in] model_viewer A pointer to the model viewer object
    public: CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, 
                            std::shared_ptr<ModelViewerI> model_viewer);

    /// \brief Implementation of interface method, wrapped by constructor
    /// \param[in] gui A pointer to the GUI object
    /// \param[in] sdformat_parser A pointer to the sdformat parser object
    /// \param[in] model_viewer A pointer to the model viewer object
    private: void Initialize(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, 
                              std::shared_ptr<ModelViewerI> model_viewer);

    /// \brief Implementation of interface method
    /// \param[in] file_path Filepath of the model
    /// \return Unique pointer to a command interface 
    private: std::unique_ptr<CommandI> MakeOpenFileCommand(std::string file_path) override;

    /// \brief Implementation of interface method
    /// \param[in] element_to_delete The SDF element to delete
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeDeleteElementCommand(sdf::ElementPtr element_to_delete) override;

    /// \brief Create an add element command
    /// \param[in] parent_element The parent of the new SDF element
    /// \param[in] new_element The new SDF element
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeAddElementCommand(
      sdf::ElementPtr parent_element, sdf::ElementPtr new_element) override;

    /// \brief Implementation of interface method
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeSaveFileCommand() override;

    /// \brief Implementation of interface method
    /// \param[in] render_collisions_in_model_viewer indicates if collision boxes should be rendered in the model viewer
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeRenderModelCommand(bool render_collisions_in_model_viewer) override;

    /// \brief Create a open model viewer model command
    /// \return Unique pointer to a command interface
    public: std::unique_ptr<CommandI> MakeOpenModelViewerCommand() override;

    /// \brief Create a close model viewer model command
    /// \return Unique pointer to a command interface
    public: std::unique_ptr<CommandI> MakeCloseModelViewerCommand() override;

    /// \brief Implementation of interface method
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeCreateFileCommand() override;

    /// \brief Implementation of interface method
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeUndoCommand() override;

    /// \brief Implementation of interface method
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeRedoCommand() override;

    /// \brief Implementation of interface method
    /// \param[in] attribute_to_modify A pointer to the attribute that will be modified
    /// \param[in] new_value The new value as a string
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, std::string new_value) override;
    
    /// \brief Implementation of interface method
    /// \param[in] attribute_to_modify A pointer to the attribute that will be modified
    /// \param[in] new_value The new value as a bool
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeModifyAttributeCommand(sdf::ParamPtr attribute_to_modify, bool new_value) override;

    /// \brief Implementation of interface method
    /// \param[in] element_to_modify A pointer to the element that will be modified
    /// \param[in] new_value The new value as a string
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeModifyElementCommand(sdf::ElementPtr element_to_modify, std::string new_value) override;

    /// \brief Implementation of interface method
    /// \param[in] element_to_modify A pointer to the element that will be modified
    /// \param[in] new_value The new value as a bool
    /// \return Unique pointer to a command interface
    private: std::unique_ptr<CommandI> MakeModifyElementCommand(sdf::ElementPtr element_to_modify, bool new_value) override;

    /// \brief Clears the undo stack
    private: void ClearStack(std::stack<std::unique_ptr<CommandI>>& stack);

    /// \brief Implementation of interface method
    private: void ClearUndoRedoStacks() override;

    /// \brief Pushes to the undo commands stack
    /// \param[in] command commandI object to push
    /// \param[in] new_change indicates if we are pushing a new change, and thus should clear the redo stack
    private: void PushToUndoCommandsStack(std::unique_ptr<CommandI> command, const bool new_change = true) override;
  
    /// \brief Pushes to the redo commands stack
    /// \param[in] command commandI object to push
    private: void PushToRedoCommandsStack(std::unique_ptr<CommandI> command) override;

    /// \brief Pops from the undo commands stack and executes the popped command
    private: void PopFromUndoCommandsStack();

    /// \brief Pops from the redo commands stack and executes the popped command
    private: void PopFromRedoCommandsStack();

    /// \brief Pointer to the GUI obejct
    private: std::shared_ptr<GUII> gui;

    /// @brief Pointer to the sdformat parser object
    private: std::shared_ptr<SDFormatParserI> sdformatParser;

    /// @brief Pointer to the model viewer object
    private: std::shared_ptr<ModelViewerI> model_viewer;

    /// @brief Stack for undo functionality
    std::stack<std::unique_ptr<CommandI>> undo_commands_stack;

    /// @brief Stack for redo functionality
    std::stack<std::unique_ptr<CommandI>> redo_commands_stack;
};

#endif