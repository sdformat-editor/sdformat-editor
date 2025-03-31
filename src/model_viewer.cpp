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

    this->scnMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light for the scene
    this->sceneLight = this->scnMgr->createLight("MainLight");
    this->sceneLightNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneLightNode->setPosition(0, 20, 0);
    this->sceneLightNode->attachObject(this->sceneLight);

    // Create a camera entity
    this->sceneCamera = this->scnMgr->createCamera("Camera");
    this->sceneCamera->setNearClipDistance(5);
    this->sceneCamera->setAutoAspectRatio(true);

    // Attach the camera to a scene node
    this->sceneCameraNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneCameraNode->attachObject(this->sceneCamera);
    this->sceneCameraNode->setPosition(0, 47, 222);
    this->sceneCameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
    
    this->ctx.getRenderWindow()->addViewport(this->sceneCamera);

    // Load some models into the scene
    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Models");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "/home/zaid/Documents/sdformat-editor/example_models/Waterwitch",
        "FileSystem",
        "Models"
    );
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Models");

    Ogre::Entity* test_entity = scnMgr->createEntity("waterwitch.stl");
    Ogre::SceneNode* test_node = scnMgr->getRootSceneNode()->createChildSceneNode();
    test_node->attachObject(test_entity);
    test_node->setScale(15, 15, 15); // Adjust scale if needed

    Ogre::Entity* test_entity_2 = scnMgr->createEntity("propeller.stl");
    Ogre::SceneNode* test_node_t = scnMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0));
    test_node_t->attachObject(test_entity_2);

    // Register for input events
    static KeyHandler keyHandler;
    this->ctx.addInputListener(&keyHandler);
}

void ModelViewer::RenderFrame(bool& should_quit)
{
  // Render a single frame
  if (!this->ogreRoot->renderOneFrame())
  {
    // Check if the escape key was pressed using the InputListener mechanism
    // This functionality is already handled in the KeyHandler class
    // Ensure KeyHandler is properly registered for input events
    should_quit = true;
    this->ctx.closeApp();
    return;
  }
  should_quit = false;
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

