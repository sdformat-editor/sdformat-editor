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

    this->active_file_path = result;

    return result;
}

// void FileOperations::WriteToModelEditorProcess(const std::string& contents)
// {
//     const char* fifo_path = "/tmp/sdf_file_editor_model_viewer_fifo";

//     // Open the FIFO for writing
//     this->model_editor_fifo_file_descriptor = open(fifo_path, O_WRONLY);
//     if (this->model_editor_fifo_file_descriptor < 0) {
//         std::cerr << "Failed to open FIFO for writing." << std::endl;
//     }
    
//     write(model_editor_fifo_file_descriptor, contents.c_str(), strlen(contents.c_str()) + 1); // Include null terminator

//     // Close the FIFO
//     close(this->model_editor_fifo_file_descriptor);
// }

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