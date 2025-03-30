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

#ifndef SDFORMAT_EDITOR_MODEL_VIEWER_INTERFACE_HH_
#define SDFORMAT_EDITOR_MODEL_VIEWER_INTERFACE_HH_

#include <iostream>
#include <GL/gl.h> // Include OpenGL header for GLuint
#include <vector>

/// \brief Interface for the SDFormat Editor's 3D Model Vewer 
class ModelViewerI
{

    /// \callgraph
    /// \brief Initialization of the Model Viewer. 
    /// NOTE: (zaid) This initalize method is not wrapped by the constructor because we can reinitalize it 
    //  multiple times (unlike the GUI), which should terminate the program if it closes.
    /// \param[out] success true if window initalization is successful
    public: virtual void Initialize(bool &success) = 0;

    /// \callgraph
    /// \param[out] should_close boolean indicating if the model viewer should close
    /// \brief Updating the Model Viewer
    public: virtual void Update(bool& should_close) = 0;

    /// \callgraph
    /// \brief Returns a render texture ID which can be rendered by the GUI implementation
    /// \returns The ID of the render texture
    public: virtual GLuint GetRenderTexture() = 0;

};

#endif