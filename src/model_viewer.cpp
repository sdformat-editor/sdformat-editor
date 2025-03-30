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

void ModelViewer::Initialize(bool &success)
{

  this->ctx.initApp();

  // get a pointer to the already created root
  this->ogreRoot = ctx.getRoot();
  this->scnMgr = this->ogreRoot->createSceneManager();

  // register our scene with the RTSS
  this->shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  // without light we would just get a black screen
  this->sceneLight = this->scnMgr->createLight("MainLight");
  this->sceneLight->setSpecularColour(0.5, 0.5, 0.5);
  this->sceneLight->setDiffuseColour(0.3, 0.3, 0.3);
  this->sceneLightNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
  this->sceneLightNode->setPosition(10, 10, 20);
  this->sceneLightNode->attachObject(this->sceneLight);

  // also need to tell where we are
  this->sceneCameraNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
  this->sceneCameraNode->setPosition(0, 0, 15);
  this->sceneCameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

  // create the camera
  this->sceneCamera = this->scnMgr->createCamera("myCam");
  this->sceneCamera->setNearClipDistance(5); // specific to this sample
  this->sceneCamera->setAutoAspectRatio(true);
  this->sceneCameraNode->attachObject(this->sceneCamera);

  // and tell it to render into the main window
  this->ctx.getRenderWindow()->addViewport(this->sceneCamera);


  // Test code for rendering waterwitch model
  // Create a resource group for external models
  Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Models");
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/home/zaid/Documents/sdformat-editor/example_models/Waterwitch", "FileSystem", "Models");
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Models");
  
  // Load a model using Assimp plugin instead of Sinbad.mesh
  // You can use various formats like .fbx, .dae, .obj, etc.
  Ogre::Entity *entity = scnMgr->createEntity("waterwitch.stl");  // Replace with your model file
  Ogre::SceneNode *node = scnMgr->getRootSceneNode()->createChildSceneNode();
  node->attachObject(entity);
  
  // Optional: Scale or position the model as needed
  node->setScale(15, 15, 15);  // Adjust scale if model is too large/small
  // node->setPosition(0, -5, 0);    // Adjust position if needed

  // register for input events
  KeyHandler keyHandler;
  ctx.addInputListener(&keyHandler);
  

  success = true;
}

void ModelViewer::Update(bool& should_close)
{
  OgreBites::WindowEventUtilities::messagePump(); // Updated namespace
  if (!this->ogreRoot->renderOneFrame())
  {
    should_close = true;
    this->ctx.closeApp();
  }
}


