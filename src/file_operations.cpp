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

#include "file_operations.h"

FileOperations& FileOperations::GetSoleInstance()
{
    static FileOperations instance;
    
    return instance;
}

std::string FileOperations::OpenFileDialog()
{
    std::array<char, 256> buffer;
    std::string result;

    FILE *pipe = popen("zenity --file-selection", "r");

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);

    // Remove trailing newline
    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }
    this->SetActiveFilePath(result);

    return result;
}

std::string FileOperations::OpenDirectoryDialog()
{
    std::array<char, 256> buffer;
    std::string result;

    FILE *pipe = popen("zenity --file-selection --directory", "r");
    // if (!pipe)
    //     return "";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);

    // Remove trailing newline
    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }
    this->SetActiveFilePath(result);

    return result;
}

void FileOperations::SetActiveFilePath(const std::string& file_path)
{
    this->active_file_path = file_path;
}

bool FileOperations::WriteFile(const std::string& contents) 
{
    return this->WriteFile(this->active_file_path, contents);
}

bool FileOperations::WriteFile(const std::string& file_path, const std::string& contents) {
    
    // NOTE: (zaid) maybe the errors here should be brought up to the GUI

    if (file_path.empty()) {
        std::cerr << "Error: File path is empty." << std::endl;
        return false;
    }

    std::ofstream file;

    file.open(file_path);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        return false;
    }

    file << contents;
    file.close();

    return true;
}