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

void ModelViewer::ModelViewerKeyHandler::addModelViewerContext(ModelViewer *m)
{
  this->m = m;
}

bool ModelViewer::ModelViewerKeyHandler::keyPressed(const OgreBites::KeyboardEvent &evt)
{
  if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
  {
    std::cout << "Hello ESC Key" << std::endl;

    Ogre::Root::getSingleton().queueEndRendering();
  }
  if (evt.keysym.sym == OgreBites::SDLK_UP)
  {
    std::cout << "Hello Up Key" << std::endl;
    m->sceneCameraNode->translate(Ogre::Vector3(0, 1, 0), Ogre::Node::TS_LOCAL);
    m->sceneCameraNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
  }
  if (evt.keysym.sym == OgreBites::SDLK_DOWN)
  {
    std::cout << "Hello Down Key" << std::endl;
    m->sceneCameraNode->translate(Ogre::Vector3(0, -1, 0), Ogre::Node::TS_LOCAL);
    m->sceneCameraNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
  }
  if (evt.keysym.sym == OgreBites::SDLK_LEFT)
  {
    std::cout << "Hello Left Key" << std::endl;
    m->sceneCameraNode->translate(Ogre::Vector3(-1, 0, 0), Ogre::Node::TS_LOCAL);
    m->sceneCameraNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
  }
  if (evt.keysym.sym == OgreBites::SDLK_RIGHT)
  {
    std::cout << "Hello Right Key" << std::endl;
    m->sceneCameraNode->translate(Ogre::Vector3(1, 0, 0), Ogre::Node::TS_LOCAL);
    m->sceneCameraNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
  }

  return true;
}

// bool ModelViewer::ModelViewerKeyHandler::mouseMoved(const OgreBites::MouseMotionEvent& evt) override
// {
//   std::cout << "Mouse moved: " << evt.x << ", " << evt.y << std::endl;
//   return true;
// }

bool ModelViewer::ModelViewerKeyHandler::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
  std::cout << "Mouse Rolled " << evt.y << std::endl;
  m->sceneCameraNode->translate(Ogre::Vector3(0, 0, -0.25 * evt.y), Ogre::Node::TS_LOCAL);
  return true;
}

void ModelViewer::Initialize()
{
    std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
    // Initialize the application context
    this->ctx.initApp();

    // Get a pointer to the Ogre root and create a scene manager
    this->ogreRoot = ctx.getRoot();
    this->scnMgr = this->ogreRoot->createSceneManager();

    // Register the scene manager with the RTShaderSystem
    this->shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // this->scnMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light for the scene
    this->sceneLight = this->scnMgr->createLight("MainLight");
    this->sceneLight->setSpecularColour(0.5, 0.5, 0.5);
    this->sceneLight->setDiffuseColour(0.3, 0.3, 0.3);
    this->sceneLightNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneLightNode->setPosition(10, 10, 20);
    this->sceneLightNode->attachObject(this->sceneLight);

    this->sceneCameraNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneCameraNode->setPosition(0, 0, 3);
    this->sceneCameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // Create a camera for rendering to the texture
    this->sceneCamera = this->scnMgr->createCamera("MyCam");
    this->sceneCamera->setNearClipDistance(1);
    this->sceneCamera->setAutoAspectRatio(true);
    this->sceneCameraNode->attachObject(this->sceneCamera);

    
    this->ctx.getRenderWindow()->addViewport(this->sceneCamera);


    // ModelInfo model = {
    //   .model_absolute_path = "/home/evanv/workspace/sdformat-editor/example_models/Waterwitch/waterwitch.stl",
    //   .pos_x = 0.0f, .pos_y = 0.0f, .pos_z = 0.0f,
    //   .rot_quaternion_w = 0.0f,
    //   .rot_quaternion_x = 1.0f,
    //   .rot_quaternion_y = 1.0f,
    //   .rot_quaternion_z = 0.0f
    // };
    // AddModel(model);

    // Register for input events
    this->keyHandler.addModelViewerContext(this);
    this->ctx.addInputListener(&this->keyHandler);
}

void ModelViewer::RenderFrame()
{
    std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
    HandleAddModelQueue();

    this->ctx.pollEvents(); 

    // Render a single frame
    if (!this->should_quit && !this->ogreRoot->renderOneFrame())
    {
      // Check if the escape key was pressed using the InputListener mechanism
      // This functionality is already handled in the ModelViewerKeyHandler class
      // Ensure ModelViewerKeyHandler is properly registered for input events
      this->should_quit = true;
      this->ctx.closeApp();
      return;
    }
}

bool ModelViewer::IsRunning()
{
    std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
    return !this->should_quit;
}

void ModelViewer::Quit()
{
    std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
    this->should_quit = true;
    this->ctx.closeApp();
}

void ModelViewer::HandleAddModelQueue() {
  while (!this->add_model_queue.empty()) {
    ModelInfo model_info = this->add_model_queue.front();
    this->add_model_queue.pop();

    std::filesystem::path file_path(model_info.model_absolute_path);
  
    std::string meshName = file_path.filename().string() + "_mesh";
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(
        meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  
    Ogre::SkeletonPtr skeleton;
    Ogre::AssimpLoader assimpLoader;
  
    // Load the mesh using AssimpLoader
    if (!assimpLoader.load(model_info.model_absolute_path, mesh.get(), skeleton, {})) {
      continue; // skip the model_info
    }
  
    // create the entity 
    Ogre::Entity* entity = this->scnMgr->createEntity(file_path.filename().string(), mesh);
  
    Ogre::SceneNode* scene_node = scnMgr->getRootSceneNode()->createChildSceneNode();
    scene_node->attachObject(entity);
  
    scene_node->setPosition(model_info.pos_x, model_info.pos_y, model_info.pos_z);
    scene_node->setOrientation(model_info.rot_quaternion_w, model_info.rot_quaternion_x, model_info.rot_quaternion_y, model_info.rot_quaternion_z);
  }
}

void ModelViewer::AddModel(ModelViewer::ModelInfo model_info) {
  std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
  add_model_queue.push(model_info);
}