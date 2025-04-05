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
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/// \brief Interface for the SDFormat Editor's 3D Model Vewer 
class ModelViewerI
{
    /// \brief Struct representing information of a model's visual information
    public: typedef struct {
        std::string model_absolute_path;
        glm::dvec3 position;
        glm::dquat orientation;
        glm::dvec3 scale = {1,1,1};
        double opacity = 1.0f;
    } ModelInfo;

    /// @brief Preset shapes which do not require a mesh to be provided in the sdf
    public: enum PresetType {
        BOX,
        CYLINDER,
        SPHERE
    };

    /// \brief Struct representing information of a model's visual information for pre-defined shapes
    public: typedef struct {
        PresetType preset_type;
        glm::dvec3 position;
        glm::dquat orientation;
        glm::dvec3 scale = {1,1,1};
        double opacity = 1.0f;
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

    /// \callgraph
    /// \brief Add a model to the model viewer, the model will be rendered on the next frame.
    /// \param[in] model_info The model information
    public: virtual void AddModel(ModelInfo model_info) = 0;

    /// \callgraph
    /// \brief Add a model to the model viewer, the model will be rendered on the next frame.
    /// \param[in] model_info The preset model information
    public: virtual void AddModel(PresetModelInfo model_info) = 0;

    /// \callgraph
    /// \brief Sets a flag in the model viewer to remove the models in the scene
    public: virtual void ResetModels() = 0;

    /// \callgraph
    /// \brief Indicates if the model viewer is running
    /// \returns a boolean
    public: virtual bool IsRunning() = 0;

    /// \callgraph
    /// \brief Method to get the model viewer's mutex
    public: virtual std::mutex& GetMutex() = 0;
};

#endif