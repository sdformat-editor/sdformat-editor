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

#include "model_viewer.h"


class KeyHandler : public OgreBites::InputListener
{
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }
};

void ModelViewer::Initialize()
{
    // Initialize the application context
    this->ctx.initApp();

    // Get a pointer to the Ogre root and create a scene manager
    this->ogreRoot = ctx.getRoot();
    this->scnMgr = this->ogreRoot->createSceneManager();

    // Register the scene manager with the RTShaderSystem
    this->shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // Create a light for the scene
    this->sceneLight = this->scnMgr->createLight("MainLight");
    this->sceneLightNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneLightNode->setPosition(10, 10, 20);
    this->sceneLightNode->attachObject(this->sceneLight);

    // Create a camera for rendering to the texture
    this->sceneCamera = this->scnMgr->createCamera("RenderTextureCamera");
    this->sceneCamera->setNearClipDistance(5);
    this->sceneCamera->setAutoAspectRatio(true);

    // Attach the camera to a scene node
    this->sceneCameraNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneCameraNode->setPosition(0, 0, 15);
    this->sceneCameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    this->sceneCameraNode->attachObject(this->sceneCamera);

    // Create a texture to render into
    this->renderTexturePointer = Ogre::TexturePtr(
        Ogre::TextureManager::getSingleton().createManual(
            "RenderTexture",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            1024, 1024, // Texture size
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        )
    );

    // Get the render target from the texture
    Ogre::RenderTexture* rtt = this->renderTexturePointer->getBuffer()->getRenderTarget();

    // Add a viewport to the render target
    Ogre::Viewport* vp = rtt->addViewport(this->sceneCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    this->sceneCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())
    );

    // Load a model into the scene
    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Models");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "/home/zaid/Documents/sdformat-editor/example_models/Waterwitch",
        "FileSystem",
        "Models"
    );
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Models");

    Ogre::Entity* entity = scnMgr->createEntity("waterwitch.stl");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setScale(15, 15, 15); // Adjust scale if needed

    // register for input events
    KeyHandler keyHandler;
    ctx.addInputListener(&keyHandler);

    // Finally, start rendering
    this->ogreRoot->startRendering();
    ctx.closeApp();
}

GLuint ModelViewer::GetRenderTexture()
{
  GLuint textureID = 0;

  // Get the hardware pixel buffer from the render texture
  Ogre::HardwarePixelBufferSharedPtr pixelBuffer = this->renderTexturePointer->getBuffer();

  // Lock the pixel buffer to access its data
  pixelBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
  const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

  // Create an OpenGL texture
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Upload the pixel data to the OpenGL texture
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    pixelBox.getWidth(),
    pixelBox.getHeight(),
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    pixelBox.data
  );

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Unlock the pixel buffer
  pixelBuffer->unlock();

  return textureID;
}

