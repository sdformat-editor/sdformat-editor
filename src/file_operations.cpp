#include "file_operations.h"
#include "sdformat_parser.h"


// FileOperations::FileOperations()
// {
//     operation_active = false;
// }

void FileOperations::OpenAndParse()
{
    if (open_dialog_thread.joinable()) {
        open_dialog_thread.join();
    }

    open_dialog_thread = std::thread(&FileOperations::SyncOpenDialog, this);
}

void FileOperations::SyncOpenDialog()
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

    // return result;
    active_file_path_mutex.lock();
    active_file_path = result;
    active_file_path_mutex.unlock();
    
    SDFormatParser* p = new SDFormatParser();
    bool b;

    p->Initialize(result, b);
}

std::string FileOperations::getActiveFilePath()
{
    std::string active_file_copy;
    active_file_path_mutex.lock();
    active_file_copy = active_file_path;
    active_file_path_mutex.unlock();
    return active_file_copy;
}