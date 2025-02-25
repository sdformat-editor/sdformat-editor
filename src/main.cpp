// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "gui.h"

// Main code
int main(int, char **)
{
    bool gui_initalization_successful;

    GUII* gui = new GUI("SDFormat Editor", gui_initalization_successful);

    if (!gui_initalization_successful) return 1;

    while (!gui->ShouldClose())
    {
        gui->Update();
    }

    // Call the destructor of gui
    delete gui;

    return 0;
}
