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

// Ogre dependencies
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"

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
  // Init the OGRE Bites app (do not use with IMGUI)
  OgreBites::ApplicationContext ctx("My App");

  ctx.initApp();

  // get a pointer to the already created root
  Ogre::Root *root = ctx.getRoot();
  Ogre::SceneManager *scnMgr = root->createSceneManager();

  // register our scene with the RTSS
  Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  // without light we would just get a black screen
  Ogre::Light *light = scnMgr->createLight("MainLight");
  light->setSpecularColour(0.5, 0.5, 0.5);
  light->setDiffuseColour(0.3, 0.3, 0.3);
  Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  lightNode->setPosition(10, 10, 20);
  lightNode->attachObject(light);

  // also need to tell where we are
  Ogre::SceneNode *camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  camNode->setPosition(0, 0, 15);
  camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

  // create the camera
  Ogre::Camera *cam = scnMgr->createCamera("myCam");
  cam->setNearClipDistance(5); // specific to this sample
  cam->setAutoAspectRatio(true);
  camNode->attachObject(cam);

  // and tell it to render into the main window
  ctx.getRenderWindow()->addViewport(cam);

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

  // Finally, start rendering
  root->startRendering();
  ctx.closeApp();
}

void ModelViewer::Update()
{
  // TODO: Implementation
}


