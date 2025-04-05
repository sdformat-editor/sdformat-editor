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

// GLM (for vector math)
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>
#include <filesystem>
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
    /// \brief Implementation of interface method
    private: void Quit() override;

    /// \callgraph
    /// \brief Indicates if the model view is running
    /// \returns a boolean
    public: bool IsRunning() override;

    /// \callgraph
    /// \brief Deallocates memory and quits the model viewer
    private: void Deinitialize() ;

    /// \brief Mutex for thread safety
    private: std::mutex model_viewer_mutex;

    /// \brief Indicates if the model viewer should quit on the next iteration
    private: bool should_quit = false;

    /// \brief Indicates if the model viewer is running
    private: bool is_running = false;

    /// \brief Implementation of interface method
    /// \param[in] model_info The model information
    public: void AddModel(ModelInfo model_info) override;

    /// \brief Implementation of interface method
    /// \param[in] model_info The preset model information
    public: void AddModel(PresetModelInfo model_info) override;

    /// \brief Implementation of the interface method
    public: void ResetModels() override;
    
    /// \brief Implementation of interface method
    private: std::mutex& GetMutex() override;

    /// \brief private method which pops ModelInfo's off the model queue and creates their OGRE entities.
    private: void HandleAddModelQueue();

    /// \brief private method to handle reset models flag
    private: void HandleResetModelsFlag();

    /// \brief The ogre application context, which gives access to attributes from the render window
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

    /// \brief queue to add models to model viewer
    private: std::queue<ModelInfo> add_model_queue;

    /// \brief class for handling inputs to the model viewer
    private:
        class ModelViewerKeyHandler : public OgreBites::InputListener
        {
            private: ModelViewer *m;
            public:void addModelViewerContext(ModelViewer *m);
            public: bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
            public: bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) override;
            public: bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
            public: bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
            public: bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
        };

    /// \brief instance of ModelViewerKeyHandler
    private: ModelViewerKeyHandler keyHandler;
    
    // private: ModelViewerWindowEventHandler windowEventHandler;
    
    // private:
    //     class ModelViewerWindowEventHandler : public OgreBites::WindowEventListener
    //     {
    //         private: ModelViewer *m;
    //         public: void addModelViewerContext(ModelViewer *m);
    //         public: void windowClosed(Ogre::RenderWindow* rw) override;
    //         public: void windowMoved(Ogre::RenderWindow* rw) override;
    //     };

    private: unsigned long long unique_naming_counter = 0;

    /// \brief list of random colours to assign model
    private: Ogre::ColourValue color_list[6] = {
        Ogre::ColourValue(3.3699f, 3.1256f, 0.633f),
        Ogre::ColourValue(0.549f, 3.1569f, 0.1569f),
        Ogre::ColourValue(4.1325f, 1.1339f, 0.234f),
        Ogre::ColourValue(1.0f, 0.5f, 0.0f), 
        Ogre::ColourValue(0.0f, 0.5f, 1.0f), 
        Ogre::ColourValue(0.5f, 0.0f, 0.5f)  
    };

    /// \brief A counter to keeps track of the current colour
    private: unsigned int color_list_index = 0;

    /// \brief if true, reset model on next frame and set false
    private: std::atomic<bool> reset_models_flag;
};

#endif