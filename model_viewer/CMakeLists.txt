cmake_minimum_required(VERSION 3.14...3.27)
project(Model-Viewer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add OGRE
option(OGRE_BUILD_COMPONENT_PYTHON "Build Python bindings" OFF)
option(OGRE_BUILD_COMPONENT_JAVA "Build Java bindings" OFF)
option(OGRE_BUILD_COMPONENT_CSHARP "Build C# bindings" OFF)
option(OGRE_BUILD_SAMPLES "Build OGRE samples" OFF)
option(OGRE_INSTALL_SAMPLES "Install OGRE samples" OFF)
option(OGRE_INSTALL_DOCS "Install documentation" OFF)
option(OGRE_INSTALL_PDB "Install debug symbols" OFF)
option(OGRE_STATIC "Build OGRE as static libraries" ON)

add_subdirectory(../libs/ogre ${CMAKE_BINARY_DIR}/ogre)

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

add_executable(model-viewer
    ${_SOURCE_FILES}
    ${HEADER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Include directories
target_include_directories(model-viewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/OgreMain/include
    ${CMAKE_BINARY_DIR}/../libs/ogre/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/Components/Bites/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/Components/RTShaderSystem/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/PlugIns/Assimp/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/Components/Overlay/include
    ${CMAKE_SOURCE_DIR}/../libs/ogre/PlugIns/Assimp/include
)

# Compiler warnings
target_compile_options(model-viewer PRIVATE
    -Wall
    -Wformat
    -g
)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

target_link_libraries(model-viewer PRIVATE
    OpenGL::GL
    glfw
    OgreMain
    OgreBites
    OgreRTShaderSystem  
    dl  
)