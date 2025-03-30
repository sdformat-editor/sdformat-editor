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

/// \brief Implementation of SDFormatParserI
class ModelViewer : public ModelViewerI
{

    /// \brief Constructor that wraps the initalize method 
    /// \NOTE: (zaid) I had to define this in the header file for some reason, 
    /// doing only the declaration here and implementation in the .cpp file is not
    /// working right away
    public: ModelViewer(bool &success) : ctx("My App") // Initialize ctx here
    {
        this->Initialize(success);
    }


    /// \brief Implementation of interface method
    private: void Initialize(bool &success) override;

    /// \brief Implementation of interface method
    private: void Update(bool& should_close) override;

    /// \brief Implementation of interface method
    public: GLuint GetRenderTexture() override; 

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

    /// \brief The texture we will be rendering into 
    private: Ogre::TexturePtr renderTexturePointer;
};

#endif