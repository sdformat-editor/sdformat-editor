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


GUI::GUI(const std::string &window_name, bool &success)
{
  std::cout << "Constructor otg" << std::endl;
  Initialize(window_name, success);
}

void GUI::glfw_error_callback(int error, const char *description)
{
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

bool GUI::ShouldClose()
{
  return glfwWindowShouldClose(this->window);
}

void GUI::Initialize(const std::string &window_name, bool &success)
{
  success = true; // Set success to be true by default

  // Specify the error handler for GLFW
  glfwSetErrorCallback(GUI::glfw_error_callback);

  // Return false if GLFW initialization fails
  if (!glfwInit()) 
  {
    success = false;
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

  // Initialize the window. The fourth and fifth arguments indicate that we are using windowed mode and 
  // not sharing resources with another window.
  // NOTE: (zaid) Can maybe determine screen resolution dynamically? 
  window = glfwCreateWindow(1280, 720, window_name.c_str(), nullptr, nullptr);

  // Return false if window initalization fails
  if (window == nullptr) 
  {
    success = false;
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
}

void GUI::Update()
{
  glfwPollEvents();
  if (glfwGetWindowAttrib(this->window, GLFW_ICONIFIED) != 0)
  {
      ImGui_ImplGlfw_Sleep(10);
      return;
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

  {
      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

      ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

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

  glfwTerminate();
}
