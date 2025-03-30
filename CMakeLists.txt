cmake_minimum_required(VERSION 3.14...3.27)
project(SDFormat-Editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(imgui_SOURCE_DIR libs/imgui)
set(stb_SOURCE_DIR libs/stb)

# Find sdformat14
find_package(sdformat14 REQUIRED)

# Add OGRE
option(OGRE_BUILD_COMPONENT_PYTHON "Build Python bindings" OFF)
option(OGRE_BUILD_COMPONENT_JAVA "Build Java bindings" OFF)
option(OGRE_BUILD_COMPONENT_CSHARP "Build C# bindings" OFF)
option(OGRE_BUILD_SAMPLES "Build OGRE samples" OFF)
option(OGRE_INSTALL_SAMPLES "Install OGRE samples" OFF)
option(OGRE_INSTALL_DOCS "Install documentation" OFF)
option(OGRE_INSTALL_PDB "Install debug symbols" OFF)
option(OGRE_STATIC "Build OGRE as static libraries" ON)

add_subdirectory(libs/ogre)

# Add header files
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
)

# Add source files
file(GLOB_RECURSE _SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
)

# Remove any file that ends with "main.cpp"
list(FILTER _SOURCE_FILES EXCLUDE REGEX ".*main\\.cpp$")

# Add ImGui files
set(IMGUI_FILES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(sdformat-editor
    ${_SOURCE_FILES}
    ${HEADER_FILES}
    ${IMGUI_FILES}
    ${stb_SOURCE_DIR}/deprecated/stb_image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Include directories
target_include_directories(sdformat-editor PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${stb_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${sdformat14_INCLUDE_DIRS}  # Add sdformat include directories
    ${CMAKE_SOURCE_DIR}/libs/ogre/OgreMain/include
    ${CMAKE_BINARY_DIR}/libs/ogre/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/Bites/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/RTShaderSystem/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/PlugIns/Assimp/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/Overlay/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/PlugIns/Assimp/include
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
    sdformat14::sdformat14
    OpenGL::GL
    glfw
    OgreMain
    OgreBites
    OgreRTShaderSystem  
    dl  
)

############################################################
######################## TESTING ###########################
############################################################

# Enable testing
enable_testing()

# Find Google Test
set(gtest_SOURCE_DIR libs/googletest)

# Add google test
add_subdirectory(${gtest_SOURCE_DIR})

# Add test files
set(TEST_FILES
    tests/test_gui.cpp
    tests/test_sdformat_parser.cpp
)

# Create test executable
add_executable(sdformat-editor-tests
    ${TEST_FILES}
    ${_SOURCE_FILES}
    ${HEADER_FILES}
    ${IMGUI_FILES}
    ${stb_SOURCE_DIR}/deprecated/stb_image.c
)

# Include directories for tests
target_include_directories(sdformat-editor-tests PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${stb_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}/googletest/include
    ${sdformat14_INCLUDE_DIRS}  # Add sdformat include directories
    ${CMAKE_SOURCE_DIR}/libs/ogre/OgreMain/include
    ${CMAKE_BINARY_DIR}/libs/ogre/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/Bites/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/RTShaderSystem/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/Components/Overlay/include
    ${CMAKE_SOURCE_DIR}/libs/ogre/PlugIns/Assimp/include
)

# Link against Google Test and other libraries
target_link_libraries(sdformat-editor-tests PRIVATE
    gtest
    gtest_main
    sdformat14::sdformat14
    OpenGL::GL
    OgreMain
    OgreBites
    OgreRTShaderSystem  
    glfw
    dl
)

# Add test to CTest
add_test(NAME sdformat-editor-tests COMMAND sdformat-editor-tests)