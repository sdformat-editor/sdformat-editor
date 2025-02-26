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

// Allows for unique pointers
#include <memory>

#include <gui_interface.h>

/// \brief Implementation of GUII
class GUI : public GUII
{

  /// \brief Constructor that wraps the Initialize method
  /// \param[in] window_name The name to be given to the SDFormatEditor Window
  /// \param[out] success true if window initalization is successful
  public: GUI(const std::string &window_name, bool &success);

  /// \brief Destructor 
  public: ~GUI();

  /// \brief Implementation of interface method
  private: void Initialize(const std::string &window_name, bool &success) override;

  /// \brief Implementation of interface method
  private: bool ShouldClose() override;

  /// \brief Implementation of update method
  private: void Update() override;
  
  /// \brief Function for handling GLFW Error (required to be static by GLFW)
  ///         NOTE: (zaid) It may be good to integrate this with an error handler class
  /// \param[in] error Error code from GLFW 
  /// \param[in] description Description of the error from GLFW 
  private: static void glfw_error_callback(int error, const char *description);

  /// \brief Pointer to the GLFW window object, used to manage for rendering and handling window events
  private: GLFWwindow *window = nullptr;

  /// @brief Reference to the ImGuiIO structure, which handles input/output operations for ImGui
  private: ImGuiIO *io = nullptr;
 
  /// @brief The background color used in the GUI
  private: ImVec4 background_colour;

};

#endif