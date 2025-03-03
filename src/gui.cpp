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

#include "gui.h"
#include "commands/OpenFileCommand.h"
#include "file_operations.h"

GUI::GUI(const std::string &window_name, std::shared_ptr<SDFormatParserI> sdformat_parser, bool &success)
{
  this->Initialize(window_name, sdformat_parser, success);
}

void GUI::GLFWErrorCallback(int error, const char *description)
{
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

bool GUI::ShouldClose()
{
  return glfwWindowShouldClose(this->window);
}

void GUI::Initialize(const std::string &window_name, std::shared_ptr<SDFormatParserI> sdformat_parser, bool &success)
{

  success = false; 

  this->sdformat_parser = sdformat_parser;

  // Specify the error handler for GLFW
  glfwSetErrorCallback(GUI::GLFWErrorCallback);

  // Return false if GLFW initialization fails
  if (!glfwInit()) 
  {
    return;
  }

  // Decide GL+GLSL versions
  #if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  #elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char *glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  #elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
  #else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
  #endif

  // Get the primary monitor and its video mode to determine screen resolution dynamically
  GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);

  // Initialize the window. The fourth and fifth arguments indicate that we are using windowed mode and 
  // not sharing resources with another window.
  window = glfwCreateWindow(video_mode->width, video_mode->height, window_name.c_str(), nullptr, nullptr);

  // Return false if window initalization fails
  if (window == nullptr) 
  {
    return;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  this->io = &ImGui::GetIO();
  (void)this->io; // Explicitly mark the io variable as used
  this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Set the background colour
  background_colour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  success = true;
}

std::string GUI::OpenFileDialog()
{
  return FileOperations::OpenFileDialog();
}


std::unique_ptr<CommandI> GUI::Update()
{
  std::unique_ptr<CommandI> command = nullptr;

  glfwPollEvents();
  if (glfwGetWindowAttrib(this->window, GLFW_ICONIFIED) != 0)
  {
      ImGui_ImplGlfw_Sleep(10);
      return command;
  }

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  
  if (ImGui::BeginMainMenuBar())
  {
      if (ImGui::BeginMenu("File"))
      {
          if (ImGui::MenuItem("Create"))
          {
          }
          if (ImGui::MenuItem("Open", "Ctrl+O"))
          {
            if (!prevent_input_flag) command = std::make_unique<OpenFileCommand>(shared_from_this(), this->sdformat_parser);
          }
          if (ImGui::MenuItem("Save", "Ctrl+S"))
          {
          }
          if (ImGui::MenuItem("Save as.."))
          {
          }
          ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
  }

  // Get the current window size
  int window_width, window_height;
  glfwGetWindowSize(this->window, &window_width, &window_height);

  // Set the position and size of the "SDFormat Editor" window
  // The window will be fixed to the right of the screen. It will take up 25% of 
  // the total width and the entire height (minus top menu bar)
  ImGui::SetNextWindowPos(ImVec2(window_width * 0.75f, 20)); // Position the window below the main menu bar
  ImGui::SetNextWindowSize(ImVec2(window_width * 0.25f, window_height - 20.0f)); // Set the dynamic size of the window

  {
    ImGui::Begin("SDFormat Editor",  nullptr, ImGuiWindowFlags_NoMove); 

    // TODO: (zaid) Save the current working directory and/or current file as an attribute to the GUI object or the file operations singleton
    // ImGui::Text("Active File is: %s", file_ops->getActiveFilePath().c_str()); // Display some text (you can use a format strings too)

    // If the user hasn't done anything so far, accept commands from the SDF element tree.
    // Otherwise, display the tree but do not take commands.
    this->DisplaySDFRootElement(command);
    

    ImGui::ColorEdit3("clear color", (float *)&this->background_colour); // Edit 3 floats representing a color

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / this->io->Framerate, this->io->Framerate);
    ImGui::End();
  }

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(this->background_colour.x * this->background_colour.w, this->background_colour.y * this->background_colour.w, this->background_colour.z * this->background_colour.w, this->background_colour.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);

  return command;
}

void GUI::DisplaySDFRootElement(std::unique_ptr<CommandI> &command)
{
  // Lock the mutex since we will be reading from the SDFormatParser
  std::lock_guard<std::mutex> lock(this->gui_mutex);

  if (ImGui::TreeNode("Node 1"))
  {
    ImGui::Text("Child 1");
    ImGui::SameLine();
    if (ImGui::Button("Modify##Child1"))
    {
      // Handle modify action for Child 1
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete##Child1"))
    {
      // Handle delete action for Child 1
    }

    ImGui::Text("Child 2");
    ImGui::SameLine();
    if (ImGui::Button("Modify##Child2"))
    {
      // Handle modify action for Child 2
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete##Child2"))
    {
      // Handle delete action for Child 2
    }

    ImGui::TreePop();
  }
  if (ImGui::TreeNode("Node 2"))
  {
    ImGui::Text("Child 1");
    ImGui::SameLine();
    if (ImGui::Button("Modify##Child1_Node2"))
    {
      // Handle modify action for Child 1 in Node 2
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete##Child1_Node2"))
    {
      // Handle delete action for Child 1 in Node 2
    }

    if (ImGui::TreeNode("SubNode"))
    {
      if (ImGui::Button("Modify"))
      {
        
      }
      ImGui::Text("SubChild 1");
      ImGui::SameLine();
      if (ImGui::Button("Modify##SubChild1"))
      {
        // Handle modify action for SubChild 1
      }
      ImGui::SameLine();
      if (ImGui::Button("Delete##SubChild1"))
      {
        // Handle delete action for SubChild 1
      }

      ImGui::Text("SubChild 2");
      ImGui::SameLine();
      if (ImGui::Button("Modify##SubChild2"))
      {
        // Handle modify action for SubChild 2
      }
      ImGui::SameLine();
      if (ImGui::Button("Delete##SubChild2"))
      {
        // Handle delete action for SubChild 2
      }

      ImGui::TreePop();
    }
    ImGui::TreePop();
  }
}

GUI::~GUI()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  if (this->window)
  {
    glfwDestroyWindow(this->window);
    this->window = nullptr;
  }

  if (this->io)
  {
    delete this->io;
    this->io = nullptr;
  }

  glfwTerminate();
}
