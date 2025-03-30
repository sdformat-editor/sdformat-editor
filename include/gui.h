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

#ifndef SDFORMAT_EDITOR_GUI_HH_
#define SDFORMAT_EDITOR_GUI_HH_

// IMGUI dependencies
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 

#include <interfaces/gui_interface.h>
#include "command_factory.h"

/// \brief Implementation of GUII. 
class GUI : public GUII
{

  /// \brief Constructor that wraps the Initialize method
  /// \param[in] window_name The name to be given to the SDFormatEditor Window
  /// \param[in] sdformat_parser Pointer to the sdformat parser object
  /// \param[in] model_viewer Pointer to the model viewer object
  /// \param[out] success true if window initalization is successful
  public: GUI(const std::string &window_name, std::shared_ptr<SDFormatParserI> sdformat_parser,
                                  std::shared_ptr<ModelViewerI> model_viewer, bool &success);

  /// \brief Destructor 
  public: ~GUI();

  /// \brief Implementation of interface method, wrapped by constructor
  private: void Initialize(const std::string &window_name, std::shared_ptr<SDFormatParserI> sdformat_parser,
                              std::shared_ptr<ModelViewerI> model_viewer, bool &success) override;

  /// \brief Implementation of interface method
  private: bool ShouldClose() override;

  /// \brief Implementation of interface method
  /// \param[in] command_factory used for creating command objects
  /// \returns The a pointer to the command resulting from the user's action during this frame
  private: std::unique_ptr<CommandI> Update(std::shared_ptr<CommandFactoryI> command_factory) override;

  /// \brief Implementation of flag setting method
  /// \param[in] set value to set the flag
  private: void SetPreventInputFlag(bool set) override;

  /// \brief Implementation of interface method 
  /// \param[in] dialogMessage struct containing the strings to display
  /// \param[out] choices a vector of string,bools pairs where one bool will be set true, corresponding to the user's choice
  public: void OpenChoiceDialog(DialogMessage dialogMessage, std::vector<std::pair<std::string, bool>>& choices) override;
  
  /// \brief Function for handling GLFW Error (required to be static by GLFW)
  ///         NOTE: (zaid) It may be good to integrate this with an error handler class
  /// \param[in] error Error code from GLFW 
  /// \param[in] description Description of the error from GLFW 
  private: static void GLFWErrorCallback(int error, const char *description);

  /// \brief Function to display the SDF root element in the GUI in a tree format 
  /// \param[out] command a pointer to the command resulting from the user's action during this frame
  /// \param[in] sdformat_parser an SDFormatParserI instance containing an sdf element
  private: void DisplaySDFRootElement(std::unique_ptr<CommandI> &command, std::shared_ptr<SDFormatParserI> sdformat_parser, 
    std::shared_ptr<CommandFactoryI> command_factory);

  /// \brief Sets up a new ImGUI frame
  /// \returns false if the window is minimized
  private: bool SetupNewFrame();

  /// \brief Draw the core part of the ImGUI frame
  /// \param[out] command a pointer to the command resulting from the user's action during this frame
  /// \param[in] command_factory used for creating command objects
  /// \returns The a pointer to the command resulting from the user's action during this frame
  private: void DrawCoreFrame(std::unique_ptr<CommandI>& command, std::shared_ptr<CommandFactoryI> command_factory);

  /// @brief Create a dropdown list 
  /// @param[in] attribute A pointer to the attribute for which to create a dropdown
  /// \param[out] command a pointer to the command resulting from the user's action during this frame
  /// \param[in] command_factory used for creating command objects
  /// \param[out] unique_id a unique id for the ImGUI dropdowm 
  private: void CreateModifyAttributeDropdown(sdf::ParamPtr attribute, std::unique_ptr<CommandI> &command, 
    std::shared_ptr<CommandFactoryI> command_factory, int& unique_id);

  /// @brief Create a dropdown list 
  /// @param[in] element A pointer to the element for which to create a dropdown
  /// \param[out] command a pointer to the command resulting from the user's action during this frame
  /// \param[in] command_factory used for creating command objects
  /// \param[out] unique_id a unique id for the ImGUI dropdowm 
  private: void CreateModifyElementDropdown(sdf::ElementPtr element, std::unique_ptr<CommandI> &command, 
    std::shared_ptr<CommandFactoryI> command_factory, int& unique_id);

  /// @brief Create a dropdown list for an element 
  /// @param[in] element A pointer to the element for which to create a dropdown
  /// \param[out] command a pointer to the command resulting from the user's action during this frame
  /// \param[in] command_factory used for creating command objects
  /// \param[out] unique_id a unique id for the ImGUI dropdowm 
  private: void CreateAppendElementDropdown(sdf::ElementPtr element, std::unique_ptr<CommandI> &command, 
    std::shared_ptr<CommandFactoryI> command_factory, int& unique_id);

  /// @brief Create a dropdown list 
  /// @param[in] items A vector of strings to include in the dropdown
  /// @param[out] selected_item an integer representing the selected item 
  /// \param[out] unique_id a unique id for the ImGUI dropdowm 
  private: void CreateDropdown(const std::vector<std::string>& items, const std::vector<std::string>& item_descriptions, int& selected_item, int& unique_id);

  /// \brief Implementation of lock method
  private: std::unique_lock<std::mutex> LockMutex() override;

  private: std::shared_ptr<CommandFactory> command_factory;

  /// \brief Flag which can be set to prevent the GUI from taking user input.
  private: std::atomic<bool> prevent_input_flag = false;
  
  /// \brief Pointer to the GLFW window object, used to manage for rendering and handling window events
  private: GLFWwindow *window = nullptr;

  /// @brief Reference to the ImGuiIO structure, which handles input/output operations for ImGui
  private: ImGuiIO *io = nullptr;

  /// @brief Pointer to the sdformat_parser object
  private: std::shared_ptr<SDFormatParserI> sdformat_parser;

  /// @brief Pointer to the sdformat_parser object
  private: std::shared_ptr<ModelViewerI> model_viewer;

  /// @brief An element for which we want to show the "append_to" dropdown
  private: sdf::ElementPtr element_to_append_to;

  /// @brief The background color used in the GUI
  private: ImVec4 background_colour;

  /// @brief Mutex to protect shared resources
  private: std::mutex gui_mutex;

  /// @brief Indicates if a dropdown is used for editing attributes
  private: bool use_dropdown_for_editing_attribute = false;

  /// @brief Holds a reference to the attribute the user is currently editing
  private: sdf::ParamPtr attribute_to_edit;

  /// @brief Holds a reference to the element the user is currently editing
  private: sdf::ElementPtr element_to_edit;

  /// @brief Indicates if a dropdown is used for editing attributes
  private: bool use_dropdown_for_editing_element = false;

};

#endif