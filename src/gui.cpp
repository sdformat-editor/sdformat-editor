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
#include "stb_image.h"
#include "commands/OpenFileCommand.h"
#include <stack>

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

void GUI::SetPreventInputFlag(bool set)
{
  this->prevent_input_flag = set;
}

std::unique_lock<std::mutex> GUI::LockMutex()
{
  return std::unique_lock<std::mutex>(this->gui_mutex);
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

  // Load image to create icon
  int icon_width, icon_height, channels;
  std::string directory = getenv("OLDPWD");
  std::string icon_file = directory + std::string("/icons/sdf_icon.png");
  stbi_uc *img = stbi_load(icon_file.c_str(), &icon_width, &icon_height, &channels, 0);
  if(img != NULL) {
    GLFWimage icon;
    icon.pixels = img;
    icon.height = icon_height;
    icon.width = icon_width;
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(img);
  } else {
    printf("Error in loading the icon. Using default\n");
  }

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

bool GUI::SetupNewFrame()
{
  glfwPollEvents();
  if (glfwGetWindowAttrib(this->window, GLFW_ICONIFIED) != 0)
  {
      ImGui_ImplGlfw_Sleep(10);
      return false;
  }

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  return true;
}

void GUI::DrawCoreFrame(std::unique_ptr<CommandI>& command, std::shared_ptr<CommandFactoryI> command_factory)
{  
  ImGuiIO& io = ImGui::GetIO();
  // Check for keyboard shortcut inputs
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_Z))
  {
    if (!(this->prevent_input_flag)) command = command_factory->MakeUndoCommand();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_Y))
  {
    if (!(this->prevent_input_flag)) command = command_factory->MakeRedoCommand();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_O))
  {
    if (!prevent_input_flag) command = command_factory->MakeOpenFileCommand("");
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_S))
  {
    if (!prevent_input_flag) command = command_factory->MakeSaveFileCommand();
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_Equal))
  {
    if (!prevent_input_flag && io.FontGlobalScale < 5.0f) io.FontGlobalScale += 0.5f;
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_Minus))
  {
    if (!prevent_input_flag && io.FontGlobalScale > 1.0f) io.FontGlobalScale -= 0.5f;
  }
  if ((ImGui::IsKeyDown(ImGuiKey_LeftCtrl)||ImGui::IsKeyDown(ImGuiKey_RightCtrl)) && ImGui::IsKeyPressed(ImGuiKey_R))
  {
    if (!prevent_input_flag) command = command_factory->MakeRenderModelCommand();
  }

  if (ImGui::BeginMainMenuBar())
  {
      if (ImGui::BeginMenu("File"))
      {
          if (ImGui::MenuItem("Create"))
          {
          }
          if (ImGui::MenuItem("Open", "Ctrl+O"))
          {
            if (!prevent_input_flag) command = command_factory->MakeOpenFileCommand("");
          }
          if (ImGui::MenuItem("Save", "Ctrl+S"))
          {
            if (!prevent_input_flag) command = command_factory->MakeSaveFileCommand();
          }
          if (ImGui::MenuItem("Save as.."))
          {
          }
          ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
      {
          if (ImGui::MenuItem("Undo", "Ctrl+Z"))
          {
            if (!(this->prevent_input_flag)) command = command_factory->MakeUndoCommand();
          }
          if (ImGui::MenuItem("Redo", "Ctrl+Y"))
          {
            if (!(this->prevent_input_flag)) command = command_factory->MakeRedoCommand();
          }
          ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Zoom"))
      {
          if (ImGui::MenuItem("Zoom in", "Ctrl + ="))
          {
            if (!prevent_input_flag && io.FontGlobalScale < 5.0f) io.FontGlobalScale += 0.5f;
          }
          if (ImGui::MenuItem("Zoom out", "Ctrl + -"))
          {
            if (!prevent_input_flag && io.FontGlobalScale > 1.0f) io.FontGlobalScale -= 0.5f;
          }
          ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Render"))
      {
          if (ImGui::MenuItem("Render Model", "Ctrl+R"))
          {
            if (!(this->prevent_input_flag)) command = command_factory->MakeRenderModelCommand();
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
  ImGui::SetNextWindowPos(ImVec2(0, 20 * io.FontGlobalScale)); // Position the window below the main menu bar
  ImGui::SetNextWindowSize(ImVec2(window_width, window_height - (20.0f * io.FontGlobalScale))); // Set the dynamic size of the window

  ImGui::Begin("SDFormat Editor",  nullptr, ImGuiWindowFlags_NoMove); 

  // TODO: (zaid) Save the current working directory and/or current file as an attribute to the GUI object or the file operations singleton
  // ImGui::TextUnformatted("Active File is: %s", file_ops->getActiveFilePath().c_str()); // Display some text (you can use a format strings too)

  // If the user hasn't done anything so far, accept commands from the SDF element tree.
  // Otherwise, display the tree but do not take commands.
  this->DisplaySDFRootElement(command, sdformat_parser, command_factory);
  
  ImGui::End();

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

std::unique_ptr<CommandI> GUI::Update(std::shared_ptr<CommandFactoryI> command_factory)
{

  std::unique_ptr<CommandI> command = nullptr;

  if (!this->SetupNewFrame())
  {
      return command;
  }

  this->DrawCoreFrame(command, command_factory);

  return command;
}

void GUI::DisplaySDFRootElement(std::unique_ptr<CommandI> &command, std::shared_ptr<SDFormatParserI> sdformat_parser, std::shared_ptr<CommandFactoryI> command_factory)
{

  // Return if there is no SDF element tree to show
  if (!sdformat_parser->GetSDFElement()) 
  {
    ImGui::TextUnformatted("SDF tree will be displayed here when a model is opened.");
    return;
  }

  // Lock the mutex since we will be reading from the SDFormatParser
  std::unique_lock<std::mutex> lock_var = this->LockMutex();
  
  // Create a stack to hold the element instances to display
  // The boolean indicates whether or not this tree node was visited
  std::stack<std::pair<sdf::ElementPtr,bool>> sdf_tree_stack;

  // To ensure that there are no issue with repeat elements for ImGui,
  // every button will be given a unique id.
  int unique_input_id = 0;
  std::map<std::string, int> unique_node_id_map = {};

  sdf::ElementPtr root_element = sdformat_parser->GetSDFElement()->Root();

  if (root_element->GetFirstElement())
  {
    // Append the root model element to the stack
    sdf_tree_stack.emplace(root_element->GetFirstElement(), false);
  }
  else if (ImGui::Button((std::string((this->element_to_append_to.get() == root_element.get()) ? "Cancel##" : "Append Element##") + std::to_string(unique_input_id++)).c_str()))
  {
    if (this->element_to_append_to.get() == root_element.get())
    {
      this->element_to_append_to.reset();
    }
    else
    {
      this->element_to_append_to = root_element;
    }
  }

  if (this->element_to_append_to.get() == root_element.get())
  {
    this->CreateAppendElementDropdown(this->element_to_append_to, command, command_factory, unique_input_id);
  }

  while (!sdf_tree_stack.empty())
  {
    // If this node has already been visited and we are returning to it, 
    // it means we have accounted it and for all of it's child nodes.
    // In that case, pop the node from the stack and end the node in the GUI
    if (sdf_tree_stack.top().second)
    {
      sdf_tree_stack.pop();
      ImGui::TreePop();
      continue;
    }

    // Get a pointer to the current node
    sdf::ElementPtr current_element_ptr = sdf_tree_stack.top().first;

    std::string node_name;
    std::string node_name_id;

    if (current_element_ptr->GetAttribute("name"))
    {
      node_name = current_element_ptr->GetName() + ": " + current_element_ptr->GetAttribute("name")->GetAsString();
    }
    else
    {
      node_name = current_element_ptr->GetName();
    }
    
    unique_node_id_map[node_name] = unique_node_id_map[node_name] + 1;
    node_name_id = node_name + "##" + std::to_string(unique_node_id_map[node_name]);
    
    // Create a unique identifier for the tree node
    if (ImGui::TreeNode(node_name_id.c_str()))
    {
      // If we are here, it means that we are exploring a new node
      // Mark the current node as visited
      sdf_tree_stack.top().second = true;

      // Delete and append buttons for this node
      if (ImGui::Button(("Delete element##" + std::to_string(unique_input_id++)).c_str()))
      {
        // Create a DeleteElement command
        if (!prevent_input_flag) command = command_factory->MakeDeleteElementCommand(current_element_ptr);
      }
      ImGui::SameLine();
      if (ImGui::Button((std::string((this->element_to_append_to.get() == current_element_ptr.get()) ? "Cancel##" : "Append Element##") + std::to_string(unique_input_id++)).c_str()))
      {
        if (this->element_to_append_to.get() == current_element_ptr.get())
        {
          this->element_to_append_to.reset();
        }
        else
        {
          this->element_to_append_to = current_element_ptr;
        }
      }

      if (this->element_to_append_to.get() == current_element_ptr.get())
      {
        this->CreateAppendElementDropdown(this->element_to_append_to, command, command_factory, unique_input_id);
      }

      // Get the current window size
      int window_width, _;
      glfwGetWindowSize(this->window, &window_width, &_);

      // Check if this element has as associated value (i.e. it is a type element)
      if (current_element_ptr->GetValue())
      {

        if (bool checkbox_state; current_element_ptr->GetValue()->IsType<bool>() && current_element_ptr->GetValue()->Get(checkbox_state)) {
          // Render checkbox
          bool original_value = checkbox_state;
          ImGui::Checkbox(("Value##" + std::to_string(unique_input_id++)).c_str(), &checkbox_state);

          if (original_value != checkbox_state) 
          {
            if (!prevent_input_flag) command = command_factory->MakeModifyElementCommand(current_element_ptr, checkbox_state);
          }
        } 
        else 
        {
          // Display the value and provide a textbox and button for modification

          std::string value_field_description = current_element_ptr->GetName() + ": " + current_element_ptr->GetValue()->GetAsString() + + " ("  + current_element_ptr->GetValue()->GetTypeName()+ ")";

          ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", value_field_description.c_str());
          static char value_buffer[128] = "";
          
          if (element_to_edit == current_element_ptr) 
          {
            if (ImGui::Button((((this->use_dropdown_for_editing_element) ? "Close Dropdown##" : "Dropdown##") + std::to_string(unique_input_id++)).c_str()))
            {
              this->use_dropdown_for_editing_element = !(this->use_dropdown_for_editing_element);
            }
            
            if (this->use_dropdown_for_editing_element)
            {
              this->CreateModifyElementDropdown(element_to_edit, command, command_factory, unique_input_id);
            }
            else
            {
              ImGui::SameLine();
              ImGui::PushItemWidth(window_width*0.1f); 
              ImGui::InputText(("##" + std::to_string(unique_input_id++)).c_str(), value_buffer, IM_ARRAYSIZE(value_buffer));
              ImGui::PopItemWidth();
              ImGui::SameLine();
  
              if (ImGui::Button(("Save##" + std::to_string(unique_input_id++)).c_str()))
              {
                if (!prevent_input_flag) command = command_factory->MakeModifyElementCommand(current_element_ptr, std::string(value_buffer));
                value_buffer[0] = '\0';
                element_to_edit.reset();
                this->use_dropdown_for_editing_element = false;
              }
            }
          } 
          else 
          {
            ImGui::SameLine();
            if (ImGui::Button(("Modify##" + std::to_string(unique_input_id++)).c_str())) 
            {
              this->element_to_edit = current_element_ptr;

              // If we are currently editing an attribute, cancel that process since we are now editing an element
              this->attribute_to_edit.reset();

              strcpy(value_buffer, current_element_ptr->GetValue()->GetAsString().c_str());
            }
          }
        }

      }
      
      // Go through each attribute of this element
      for (const auto &attribute_ptr : current_element_ptr->GetAttributes())
      {
        
        bool attribute_value;
        std::string attribute_info = attribute_ptr->GetKey() + ": " +  attribute_ptr->GetAsString() + " ("  + attribute_ptr->GetTypeName()+ ")";

        if (attribute_ptr->IsType<bool>() && attribute_ptr->Get(attribute_value)) 
        {
          // Render checkbox
          bool original_value = attribute_value;
          ImGui::Checkbox(("##" + std::to_string(unique_input_id++)).c_str(), &attribute_value);
          ImGui::SameLine();
          ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s", attribute_info.c_str());
          
          if (original_value != attribute_value) 
          {
            if (!prevent_input_flag) command = command_factory->MakeModifyAttributeCommand(attribute_ptr, attribute_value);
          }
        } 
        else 
        {
          ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s", attribute_info.c_str());
          static char value_buffer[1024] = "";
          if (attribute_to_edit == attribute_ptr) {

            if (ImGui::Button((((this->use_dropdown_for_editing_attribute) ? "Close Dropdown##" : "Dropdown##") + std::to_string(unique_input_id++)).c_str()))
            {
              this->use_dropdown_for_editing_attribute = !(this->use_dropdown_for_editing_attribute);
            }

            if (this->use_dropdown_for_editing_attribute)
            {
              this->CreateModifyAttributeDropdown(this->attribute_to_edit, command, command_factory, unique_input_id);
            }
            else
            {
              ImGui::SameLine();
              ImGui::PushItemWidth(window_width*0.1f); 
              ImGui::InputText(("##" + std::to_string(unique_input_id++)).c_str(), value_buffer, IM_ARRAYSIZE(value_buffer));
              ImGui::PopItemWidth();
              ImGui::SameLine();
              
              if (ImGui::Button(("Save##" + std::to_string(unique_input_id++)).c_str()))
              {
                if (!prevent_input_flag) command = command_factory->MakeModifyAttributeCommand(attribute_ptr, std::string(value_buffer));
                value_buffer[0] = '\0';
                this->attribute_to_edit.reset();        
                this->use_dropdown_for_editing_attribute = false;
              }
            }
          } 
          else 
          {
            ImGui::SameLine();
            if (ImGui::Button(("Modify##" + std::to_string(unique_input_id++)).c_str())) 
            {
              this->attribute_to_edit = attribute_ptr;
              this->element_to_edit.reset();
              strcpy(value_buffer, attribute_ptr->GetAsString().c_str());
            }
          }
        }

      }

      if (current_element_ptr->GetFirstElement())
      {
        // Go through each element 
        sdf::ElementPtr child_element_ptr = current_element_ptr->GetFirstElement();
        while (child_element_ptr)
        {
          // Add this child element to the stack
          sdf_tree_stack.emplace(child_element_ptr, false);
          child_element_ptr = child_element_ptr->GetNextElement("");
        }
      }
      else
      {
        // If this element has no children (leaf node), then it may be removed from the stack
        sdf_tree_stack.pop();
        ImGui::TreePop();
        continue;
      }
    }
    else
    {
      // This element's tree node is not open, so its contents
      // should not be rendered
      sdf_tree_stack.pop();
      continue;
    }
  }
}

void GUI::CreateModifyAttributeDropdown(sdf::ParamPtr attribute, std::unique_ptr<CommandI> &command, std::shared_ptr<CommandFactoryI> command_factory, int& unique_id)
{
    std::vector<std::string> attribute_names;
    std::vector<std::string> attribute_names_with_id;
  
    attribute_names.push_back("##");
    attribute_names_with_id.push_back("##");
  
    std::vector<sdf::ElementPtr> elements_with_name_attribute = this->sdformat_parser->LookupElementsByAttributeType("name");

    for (const auto &element : elements_with_name_attribute)
    {
      if (element->GetAttribute("name") && 
          std::find(attribute_names.begin(), attribute_names.end(), element->GetAttribute("name")->GetAsString()) == attribute_names.end())
      {
        attribute_names.push_back(element->GetAttribute("name")->GetAsString());
        attribute_names_with_id.push_back(element->GetAttribute("name")->GetAsString()+"##"+std::to_string(unique_id++));
      }
    }
    
    int selected_attribute = 0;
    this->CreateDropdown(attribute_names_with_id, std::vector<std::string>(), selected_attribute, unique_id);
  
    if ((0 < selected_attribute) && (static_cast<size_t>(selected_attribute) < attribute_names.size()))
    {
      if (!prevent_input_flag) 
      {
        command = command_factory->MakeModifyAttributeCommand(attribute, attribute_names[selected_attribute]);
        this->attribute_to_edit.reset();
      }
    }
}

void GUI::CreateModifyElementDropdown(sdf::ElementPtr element, std::unique_ptr<CommandI> &command, std::shared_ptr<CommandFactoryI> command_factory, int& unique_id)
{
    std::vector<std::string> element_names;
    std::vector<std::string> element_names_with_id;
  
    element_names.push_back("##");
    element_names_with_id.push_back("##");
  
    std::vector<sdf::ElementPtr> elements_with_name_attribute = this->sdformat_parser->LookupElementsByAttributeType("name");

    for (const auto &element : elements_with_name_attribute)
    {
      if (element->GetAttribute("name") && 
          std::find(element_names.begin(), element_names.end(), element->GetAttribute("name")->GetAsString()) == element_names.end())
      {
        element_names.push_back(element->GetAttribute("name")->GetAsString());
        element_names_with_id.push_back(element->GetAttribute("name")->GetAsString()+"##"+std::to_string(unique_id++));
      }
    }
    
    int selected_element = 0;
    this->CreateDropdown(element_names_with_id, std::vector<std::string>(), selected_element, unique_id);
  
    if ((0 < selected_element) && (static_cast<size_t>(selected_element) < element_names.size()))
    {
      if (!prevent_input_flag) 
      {
        command = command_factory->MakeModifyElementCommand(element, element_names[selected_element]);
        this->attribute_to_edit.reset();
      }
    }
}

void GUI::CreateAppendElementDropdown(sdf::ElementPtr element, std::unique_ptr<CommandI> &command, std::shared_ptr<CommandFactoryI> command_factory, int& unique_id)
{
    std::vector<std::string> element_names;
    std::vector<std::string> element_descriptions;
  
    element_names.push_back("##");
    element_descriptions.push_back("");
  
    for (size_t i = 0; i < element->GetElementDescriptionCount(); i++)
    {
      element_names.push_back(element->GetElementDescription(i)->GetName());
      element_descriptions.push_back(element->GetElementDescription(i)->GetDescription());
    }
  
    // Add an option to create a custom element
    element_names.push_back("I want to create a custom element");
    element_descriptions.push_back("Open a dialog to create a custom element");
  
    int selected_element = 0;
    this->CreateDropdown(element_names, element_descriptions, selected_element, unique_id);
  
    // Use a static cast to prevent compiler warning
    if (static_cast<size_t>(selected_element) > element->GetElementDescriptionCount())
    {
      // Make an add element command where the new_element parameter is null, indicating this is an entirely new element 
      if (!prevent_input_flag) {
        command = command_factory->MakeAddElementCommand(element, nullptr);
        this->element_to_append_to.reset();
      }
    }
    else if (selected_element > 0)
    {
      if (!prevent_input_flag) {
        sdf::ElementPtr child_element = element->GetElementDescription(selected_element-1)->Clone();
        command = command_factory->MakeAddElementCommand(element, child_element);
        this->element_to_append_to.reset();
      }
    }
}

void GUI::CreateDropdown(const std::vector<std::string>& items, const std::vector<std::string>& item_descriptions, int& selected_item, int& unique_id)
{
  static std::string item_current = items[0];            // Here our selection is a single pointer stored outside the object.
  if (ImGui::BeginCombo(("##"+std::to_string(unique_id++)).c_str(), item_current.c_str())) // The second parameter is the label previewed before opening the combo.
  {
      for (size_t n = 0; n < items.size(); n++)
      {
        bool is_selected = (item_current == items[n]);
        if (ImGui::Selectable(items[n].c_str(), is_selected))
        {
            selected_item = static_cast<int>(n);
            ImGui::EndCombo();
            item_current = "##";
            return;
        }
        if (is_selected)
        {
            ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        
        // Check if the current item is hovered
        if (ImGui::IsItemHovered())
        {
            if (item_descriptions.size()==items.size())
            {
              ImGui::BeginTooltip();
              int window_width, _;
              glfwGetWindowSize(this->window, &window_width, &_);
              ImGui::PushTextWrapPos(window_width * 0.3f); // Limit the width of the tooltip to 30% of the window width
              ImGui::TextWrapped("%s", item_descriptions[n].c_str());
              ImGui::PopTextWrapPos();
              ImGui::EndTooltip();
            }
            item_current = items[n];
        }
      }

      ImGui::EndCombo();
  }
}

void GUI::OpenChoiceDialog(DialogMessage dialogMessage, std::vector<std::pair<std::string, bool>>& choices)
{
  this->SetPreventInputFlag(true);

  bool user_responded = false;
  
  while (!(this->ShouldClose() || user_responded))
  {

    if (!this->SetupNewFrame())
    {
        continue;
    }

    ImGui::OpenPopup(dialogMessage.header.c_str());

    if (ImGui::BeginPopupModal(dialogMessage.header.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::SetWindowFontScale(1.0f); 
      ImGui::Text("%s", dialogMessage.body.c_str());

      ImGui::SetWindowFontScale(1.1f); 
      ImGui::Text("%s", dialogMessage.footer.c_str());

      ImGui::SetWindowFontScale(1.0f); 
      ImGui::Separator();

      for (auto &choice : choices)
      {
        if (ImGui::Button(choice.first.c_str()))
        {
          user_responded = true;
          choice.second = true;
          ImGui::CloseCurrentPopup();
          break;
        }
        ImGui::SameLine();
      }

      ImGui::EndPopup();
    }
    std::unique_ptr<CommandI> dummy_command = nullptr;

    // Passing in nullptr for the command factory won't cause an issue since prevent_input_flag is true. 
    // However, it should be strictly enforced that the command_factory argument is only used when
    // the prevent_input_flag is false.
    this->DrawCoreFrame(dummy_command, nullptr);
  }
  
  this->SetPreventInputFlag(false);
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
    this->io = nullptr;
  }

  glfwTerminate();
}