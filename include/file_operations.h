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

// SDFormat dependencies
#include <sdf/sdf.hh>

#include <cstdlib>
#include <cstring>
#include <limits.h>
#include <memory>
#include <thread>
#include <mutex>

class FileOperations
{
    private: bool operation_active;

    private: std::thread open_dialog_thread;

    private: std::string active_file_path = "";
    private: std::mutex active_file_path_mutex;

    public: std::string getActiveFilePath();

    public: void OpenAndParse();

    private: void SyncOpenDialog();
};

#endif