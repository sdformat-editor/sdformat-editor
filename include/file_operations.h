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

#ifndef FILE_OPERATIONS_HH_
#define FILE_OPERATIONS_HH_

#include <string>
#include <iostream>
#include <array>
#include <fstream>

/// \brief Singleton class to handle file operations
class FileOperations
{

    /// \callgraph
    /// \brief The only method by which a FileOperations instance can be returned.
    // NOTE: Singleton Implementation based on Mayers Singleton for C++11 and beyond
    // See https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton/ 
    /// \returns The sole instance of FileOperationsI 
    public: static FileOperations& GetSoleInstance();

    /// \callgraph
    /// \brief Opens a dialog for the user to open a file on their filesystem 
    /// \return An absolute file path or an empty string
    public: std::string OpenFileDialog();

    /// \callgraph
    /// \brief writes a string to a file
    /// \param[in] file_path the filepath of the file to write to
    /// \param[in] contents the contents to write to the given file
    /// \returns true if successful 
    public: bool WriteFile(const std::string& file_path, const std::string& contents);

    /// \callgraph
    /// \brief writes a string to a file
    /// \param[in] contents the contents to write to the given file
    /// \returns true if successful 
    public: bool WriteFile(const std::string& contents);

    /// \callgraph
    /// \brief Prevent access to the constructor of this class
    private: FileOperations() = default;

    /// \callgraph
    /// \brief Prevent access to the destructor of this class
    private: ~FileOperations() = default;

    /// \callgraph
    /// \brief Prevent the assignment of one instance of FileOperations to another, 
    /// ensuring there can only be one instance
    private: FileOperations& operator = (const FileOperations&) = delete;

    /// \callgraph
    /// @brief Delete the copy constructor of FileOperations, preventing copies 
    /// of the reference the the sole instance from being made  
    private: FileOperations(const FileOperations&) = delete;

    private: std::string active_file_path;

    private: int model_editor_fifo_file_descriptor;
};

#endif