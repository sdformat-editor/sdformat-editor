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

#ifndef SDFORMAT_EDITOR_MODEL_VIEWER_HH_
#define SDFORMAT_EDITOR_MODEL_VIEWER_HH_

#include <model_viewer_interface.h>

/// \brief Implementation of SDFormatParserI
class ModelViewer : public ModelViewerI
{

    /// \brief Constructor that wraps the Initialize method
    /// \param[in] window_name The name to be given to the SDFormatEditor Window
    /// \param[out] success true if window initalization is successful
    public: ModelViewer(const std::string &window_name, bool &success);

    /// \brief Destructor 
    public: ~ModelViewer();

    /// \brief Implementation of interface method
    private: void Initialize(const std::string &window_name, bool &success) override;

    /// \brief Implementation of update method
    private: void Update() override;

};

#endif