cmake_minimum_required(VERSION 3.14)
project(SDFormat-Editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(imgui_SOURCE_DIR libs/imgui)

# Find sdformat14
find_package(sdformat14 REQUIRED)

# message(STATUS "sdformat14_INCLUDE_DIRS: ${sdformat14_INCLUDE_DIRS}")

# Add gui files
set(GUI_FILES
    include/interfaces/gui_interface.h
    include/gui.h
    src/gui.cpp

    src/file_operations.cpp
    include/file_operations.h
)
# Add gui files
set(SDFORMAT_PARSER_FILES
    include/interfaces/sdformat_parser_interface.h
    include/sdformat_parser.h
    src/sdformat_parser.cpp
)

# Add gui files
set(MODEL_VIEWER_FILES
    include/interfaces/model_viewer_interface.h
    include/model_viewer.h
    src/model_viewer.cpp
)

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
    src/main.cpp
    ${GUI_FILES}
    ${SDFORMAT_PARSER_FILES}
    ${MODEL_VIEWER_FILES}
    ${IMGUI_FILES}
)

# Include directories
target_include_directories(sdformat-editor PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${sdformat14_INCLUDE_DIRS}  # Add sdformat include directories
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
    tests/test_model_viewer.cpp
)

# Create test executable
add_executable(sdformat-editor-tests
    ${TEST_FILES}
    ${GUI_FILES}
    ${SDFORMAT_PARSER_FILES}
    ${MODEL_VIEWER_FILES}
    ${IMGUI_FILES}
)

# Include directories for tests
target_include_directories(sdformat-editor-tests PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}/googletest/include
    ${sdformat14_INCLUDE_DIRS}  # Add sdformat include directories
)

# Link against Google Test and other libraries
target_link_libraries(sdformat-editor-tests PRIVATE
    gtest
    gtest_main
    sdformat14::sdformat14
    OpenGL::GL
    glfw
    dl
)

# Add test to CTest
add_test(NAME sdformat-editor-tests COMMAND sdformat-editor-tests)