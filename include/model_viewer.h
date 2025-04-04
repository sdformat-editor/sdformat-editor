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

#include <interfaces/model_viewer_interface.h>

// Ogre dependencies
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreWindowEventUtilities.h"
#include "OgreAssimpLoader.h"
#include "OgreCameraMan.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <mutex>
#include <queue>

/// \brief Implementation of SDFormatParserI
class ModelViewer : public ModelViewerI
{

    /// \brief Constructor
    public: ModelViewer() : ctx("SDFormat Editor Model Viewer") // Initialize ctx here
    {
    }

    /// \brief Implementation of interface method
    private: void Initialize() override;

    /// \brief Implementation of interface method
    private: void RenderFrame() override;

    /// \callgraph
    /// \brief Tells the model editor to quit on its next iteration
    private: void Quit() override;

    /// \callgraph
    /// \brief Indicates if the model view is running
    /// \returns a boolean
    public: bool IsRunning() override;

    /// \brief Mutex for thread safety
    private: std::mutex model_viewer_mutex;

    /// \brief Mutex for thread safety
    private: bool should_quit = false;

    /// \brief Implementation of interface method
    /// \param[in] model_info struct containing the model information
    public: void AddModel(ModelInfo model_info) override;
    public: void AddModel(PresetModelInfo model_info) override;

    /// \brief Implementation of the interface method
    public: void ResetModels() override;

    /// \brief private method which pops ModelInfo's off the model queue and creates their OGRE entities.
    private: void HandleAddModelQueue();

    /// brief private method to handle reset models flag
    private: void HandleResetModelsFlag();

    /// \brief The ogre bites app (i think we need to replace this with smthn else when we integrate with imgui)
    private: OgreBites::ApplicationContext ctx;

    /// \brief Pointer to the ogre root
    private: Ogre::Root* ogreRoot;

    /// \brief Pointer to the ogre scene manager
    private: Ogre::SceneManager* scnMgr;

    /// \brief Pointer to the ogre shader generator singleton
    private: Ogre::RTShader::ShaderGenerator *shadergen;

    /// \brief Pointer to the main scene light
    private: Ogre::Light *sceneLight;

    /// \brief Pointer to the main scene light node
    private: Ogre::SceneNode *sceneLightNode;

    /// \brief Pointer to the main scene camera
    private: Ogre::Camera *sceneCamera;

    /// \brief Pointer to the main scene camera node
    private: Ogre::SceneNode* sceneCameraNode;

    /// \brief Pointer to the main camera controller
    private: OgreBites::CameraMan* cameraController;

    /// \brief The texture we will be rendering into 
    private: Ogre::TexturePtr renderTexturePointer;

    /// \brief queue to add models to model viewer
    private: std::queue<ModelInfo> add_model_queue;

    /// \brief class for handling inputs to the model viewer
    private:
        class ModelViewerKeyHandler : public OgreBites::InputListener
        {
            private: ModelViewer *m;
            public:void addModelViewerContext(ModelViewer *m);
            // most of these are just to call the methods of CameraMan
            public: bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
            public: bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) override;
            public: bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
            public: bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
            public: bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
        };
    
    /// \brief instance of ModelViewerKeyHandler
    private: ModelViewerKeyHandler keyHandler;

    /// @brief 
    private: unsigned long long unique_naming_counter = 0;

    /// \brief list of random colours
    private: Ogre::ColourValue color_list[3] = {
        Ogre::ColourValue(3.3699f, 3.1256f, 0.633f),
        Ogre::ColourValue(0.549f, 3.1569f, 0.1569f),
        Ogre::ColourValue(4.1325f, 1.1339f, 0.234f),
        // Ogre::ColourValue(),
        // Ogre::ColourValue()
    };
    private: unsigned int color_list_index = 0;

    /// \brief if true, reset model on next frame and set false
    private: std::atomic<bool> reset_models_flag;
};

#endif