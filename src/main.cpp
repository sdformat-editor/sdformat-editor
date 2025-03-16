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

#include "file_editor_runner.h"
#include <sys/stat.h>

// Entry point
int main(int, char **)
{
    bool data_dir_created = true;
    // Get the home directory
    const char* home_dir = std::getenv("HOME");
    if (home_dir == nullptr) {
        std::cerr << "Could not find home directory" << std::endl;
        data_dir_created = false;
    }

    // Construct the path to the data directory
    std::string data_dir = std::string(home_dir) + "/.local/share/sdformat_editor/";

    // Check if the directory exists, if not, create it
    struct stat info;
    if (stat(data_dir.c_str(), &info) != 0) {
        std::cout << "Directory doesn't exist, creating: " << data_dir << std::endl;
        if (mkdir(data_dir.c_str(), 0700) != 0) {
            std::cerr << "Failed to create directory!" << std::endl;
            data_dir_created = false;
        }
    }

    // Create instance of main program
    FileEditorRunner program(data_dir_created);

    // Status of termination
    int status;

    status = program.RunProgram();

    return status;

}
