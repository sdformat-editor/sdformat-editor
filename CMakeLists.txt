cmake_minimum_required(VERSION 3.14)
project(SDFormat-Editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(imgui_SOURCE_DIR libs/imgui)

# Find sdformat14
find_package(sdformat14 REQUIRED)

# Add gui files
set(GUI_FILES
    include/gui_interface.h
    include/gui.h
    src/gui.cpp
)

add_executable(sdformat-editor
    src/main.cpp
    ${GUI_FILES}
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(sdformat-editor PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against sdformat14
target_link_libraries(sdformat-editor PRIVATE
    sdformat14::sdformat14
)

# Compiler warnings
target_compile_options(sdformat-editor PRIVATE
    -Wall
    -Wformat
    -g
)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(sdformat-editor PRIVATE
    OpenGL::GL
    glfw
    dl  # Add dynamic loading library
)