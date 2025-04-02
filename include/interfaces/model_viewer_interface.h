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
#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/// \brief Interface for the SDFormat Editor's 3D Model Vewer 
class ModelViewerI
{
    public: typedef struct {
        std::string model_absolute_path;
        
        // Position Vector
        glm::dvec3 position;
        
        // Orientation Quaternion
        glm::dquat orientation;

        // Scale
        glm::dvec3 scale = {1,1,1};

        float opacity = 1.0f;
    } ModelInfo;

    public: typedef enum PresetType {
        BOX,
        CYLINDER
    };

    public: typedef struct {
        PresetType preset_type;
        
        // Position Vector
        glm::dvec3 position;
        
        // Orientation Quaternion
        glm::dquat orientation;

        // Scale
        glm::dvec3 scale = {1,1,1};

        float opacity = 1.0f;
    } PresetModelInfo;




    /// \callgraph
    /// \brief Initialization of the Model Viewer. 
    public: virtual void Initialize() = 0;

    /// \callgraph
    /// \brief Rander a single frame
    public: virtual void RenderFrame() = 0;

    /// \callgraph
    /// \brief Tells the model viewer to quit on its next iteration
    public: virtual void Quit() = 0;
    
    public: virtual void AddModel(ModelInfo) = 0;
    virtual void AddModel(PresetModelInfo) = 0;
    
    /// \callgraph
    /// \brief Indicates if the model view is running
    /// \returns a boolean
    public: virtual bool IsRunning() = 0;
};

#endif