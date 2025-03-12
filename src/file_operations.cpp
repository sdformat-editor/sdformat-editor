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

    return result;
    
}

