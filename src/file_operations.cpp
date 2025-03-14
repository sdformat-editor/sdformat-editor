#include "file_operations.h"

#include <fstream>
#include <iostream>

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

    this->active_file_path = result;

    return result;
}

void FileOperations::WriteFile(const std::string& contents) {
    this->WriteFile(this->active_file_path, contents);
}

void FileOperations::WriteFile(const std::string& file_path, const std::string& contents) {
    
    // NOTE: (zaid) maybe the errors here should be brought up to the GUI
  
    if (file_path.empty()) {
        std::cerr << "Error: File path is empty." << std::endl;
        return;
    }

    std::ofstream file;

    file.open(file_path);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        return;
    }

    file << contents;
    file.close();
}