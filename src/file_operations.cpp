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

void FileOperations::WriteFile(std::string contents) {
    std::ofstream file;
    file.open(this->active_file_path);
    file << contents;
    file.close();
}

void FileOperations::WriteFile(std::string file_path, std::string contents) {
    std::ofstream file;
    file.open(file_path);
    file << contents;
    file.close();
}