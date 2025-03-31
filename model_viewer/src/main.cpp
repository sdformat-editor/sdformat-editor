#include "model_viewer.h"

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring> // For memset

int main(int argc, char** argv)
{
    // Create and open a FIFO
    const char* fifo_path = "/tmp/sdf_file_editor_model_viewer_fifo";

    // Create the FIFO if it doesn't exist
    if (mkfifo(fifo_path, 0666) < 0 && errno != EEXIST) {
        std::cerr << "Failed to create FIFO: " << strerror(errno) << std::endl;
        return 1;
    }

    int fifo_file_descriptor = open(fifo_path, O_RDONLY | O_NONBLOCK); // Open FIFO in non-blocking mode
    if (fifo_file_descriptor < 0)
    {
        std::cerr << "Failed to open FIFO for reading." << std::endl;
        return 1;
    }

    // Initialize the model viewer

    ModelViewerI* model_viewer = new ModelViewer();
    model_viewer->Initialize();

    bool should_quit = false;

    while (!should_quit)
    {
        // Read from the FIFO
        char buffer[128];
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        ssize_t bytes_read = read(fifo_file_descriptor, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0)
        {
            std::cout << "Received from FIFO: " << buffer << std::endl;

            // Example: Check for a quit command
            if (std::string(buffer) == "quit")
            {
                should_quit = true;
            }
            else if (std::string(buffer) == "render_frame")
            {
                // Call the RenderFrame method
                model_viewer->RenderFrame(should_quit);
            }
        }
    }

    // Clean up
    close(fifo_file_descriptor);
    unlink(fifo_path); // Remove the FIFO file
    delete model_viewer;

    return 0;
}