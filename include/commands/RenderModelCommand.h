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

#ifndef SDFORMAT_EDITOR_RENDER_MODEL_COMMAND_HH_
#define SDFORMAT_EDITOR_RENDER_MODEL_COMMAND_HH_


#include <interfaces/command_interface.h>
#include <interfaces/model_viewer_interface.h>
#include <interfaces/sdformat_parser_interface.h>
#include <thread>


/// \brief Render Model command implementation of CommandI
class RenderModelCommand : public CommandI
{

  /// \callgraph
  /// \brief Constructor for render model command objects.
  /// \param[in] model_viewer A pointer to the model viewer object
  /// \param[in] sdformat_parser A pointer to the SDFormat parser object
  /// \param[in] render_collisions_in_model_viewer A boolean to indicate wether or not to render collision objects
  public: RenderModelCommand(std::shared_ptr<ModelViewerI> model_viewer, std::shared_ptr<SDFormatParserI> sdformat_parser, bool render_collisions_in_model_viewer);

  /// \brief Implementation of interface method. 
  /// \returns This command calls a method from the SDFormat Parser to obtain information about all the models in the sdf tree
  /// and then adds each model to a render queue on the model viewer, which runs on a seperate thread
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
  /// \param[out] prevent_user_input indicates if user input should be prevented 
  /// if this happens to be a threaded command
  /// \returns Always false
  private: bool IsThreaded(bool& prevent_user_input) override;

  /// \brief Implementation of interface method.
  /// \returns Always false
  private: bool ChangesProgramStateIrreversibly() override;

  /// @brief Pointer to the model viewer object
  private: std::shared_ptr<ModelViewerI> model_viewer;

  /// @brief Pointer to the SDFormat parser object
  private: std::shared_ptr<SDFormatParserI> sdformat_parser;

  /// \brief Indicates if collisions should be rendered in the model viewer
  private: bool render_collisions_in_model_viewer;

};

#endif