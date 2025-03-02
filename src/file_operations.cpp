#include "file_operations.h"


// FileOperations::FileOperations()
// {
//     operation_active = false;
// }

// void FileOperations::OpenAndParse()
// {
//     if (open_dialog_thread.joinable()) {
//         open_dialog_thread.join();
//     }

//     open_dialog_thread = std::thread(&FileOperations::SyncOpenDialog, this);
// }

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
    // active_file_path_mutex.lock();
    // active_file_path = result;
    // active_file_path_mutex.unlock();
    
    // SDFormatParser* p = new SDFormatParser();
    // bool b;

    // p->Initialize(result, b);
}

