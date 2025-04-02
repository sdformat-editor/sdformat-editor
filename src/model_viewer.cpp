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
  return true;
}

bool ModelViewer::ModelViewerKeyHandler::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
  m->cameraController->mouseMoved(evt);
  return true;
}

bool ModelViewer::ModelViewerKeyHandler::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
  m->sceneCameraNode->translate(Ogre::Vector3(0, 0, -0.25 * evt.y), Ogre::Node::TS_LOCAL);
  return true;
}

bool ModelViewer::ModelViewerKeyHandler::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
  m->cameraController->mousePressed(evt);
  return true;
}

bool ModelViewer::ModelViewerKeyHandler::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
  m->cameraController->mouseReleased(evt);
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

    this->sceneCameraNode = this->scnMgr->getRootSceneNode()->createChildSceneNode();
    this->sceneCameraNode->setPosition(0, 0, 3);
    this->sceneCameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // Create a light for the scene
    this->sceneLight = this->scnMgr->createLight("MainLight");
    this->sceneLight->setSpecularColour(0.5, 0.5, 0.5);
    this->sceneLight->setDiffuseColour(0.3, 0.3, 0.3);
    this->sceneLightNode = this->sceneCameraNode->createChildSceneNode();
    this->sceneLightNode->setPosition(10, 10, 20);
    this->sceneLightNode->attachObject(this->sceneLight);

    // Create a camera for rendering to the texture
    this->sceneCamera = this->scnMgr->createCamera("MyCam");
    this->sceneCamera->setNearClipDistance(0.1);
    this->sceneCamera->setAutoAspectRatio(true);
    this->sceneCameraNode->attachObject(this->sceneCamera);
    
    // Create a cameraman to control the camera
    this->cameraController = new OgreBites::CameraMan(this->sceneCameraNode);
    this->cameraController->setStyle(OgreBites::CameraStyle::CS_ORBIT);

    this->ctx.getRenderWindow()->addViewport(this->sceneCamera);
    this->ctx.getRenderWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue(0.45f, 0.55f, 0.60f));

    // ModelInfo model = {
    //   .model_absolute_path = "/home/evanv/workspace/sdformat-editor/models/Waterwitch/waterwitch.stl",
    //   .position = {0.0f, 0.0f, 0.0f},
    //   .orientation = {0.0f, 1.0f, 0.0f, 0.0f},
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
  
    std::string meshName = file_path.filename().string() + "_mesh_" + std::to_string(unique_naming_counter++);
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(
        meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  
    Ogre::SkeletonPtr skeleton;
    Ogre::AssimpLoader assimpLoader;
  
    // Load the mesh using AssimpLoader
    if (!assimpLoader.load(model_info.model_absolute_path, mesh.get(), skeleton, {})) {
      continue; // skip the model_info
    }
  
    // create the entity 
    Ogre::Entity* entity = this->scnMgr->createEntity(file_path.filename().string() + std::to_string(unique_naming_counter++), mesh);
  
    Ogre::SceneNode* scene_node = scnMgr->getRootSceneNode()->createChildSceneNode();
    scene_node->attachObject(entity);
  
    scene_node->setPosition(model_info.position.x, model_info.position.y, model_info.position.z);
    scene_node->setOrientation(model_info.orientation.w, model_info.orientation.x, model_info.orientation.y, model_info.orientation.z);
    scene_node->setScale(model_info.scale.x, model_info.scale.y, model_info.scale.z);
    
    // Set the opacity of our model
    Ogre::MaterialPtr material = entity->getSubEntity(0)->getMaterial();

    // Don't modify the existing material, create a new one
    Ogre::MaterialPtr new_material = material->clone(file_path.filename().string() + "_mat_" + std::to_string(unique_naming_counter++));
    new_material->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    // new_material->setDepthWriteEnabled(false);

    new_material->setDiffuse(color_list[color_list_index].r, color_list[color_list_index].g, color_list[color_list_index].b, model_info.opacity);
    new_material->setAmbient(color_list[color_list_index]);
    color_list_index = (color_list_index + 1) % std::size(color_list);
    new_material->setShadingMode(Ogre::SO_FLAT);

    entity->getSubEntity(0)->setMaterial(new_material);
  }
}

void ModelViewer::AddModel(ModelViewer::ModelInfo model_info) {
  std::lock_guard<std::mutex> lock(this->model_viewer_mutex);
  add_model_queue.push(model_info);
}

void ModelViewer::AddModel(ModelViewer::PresetModelInfo model_info) {
  std::lock_guard<std::mutex> lock(this->model_viewer_mutex);

  ModelViewer::ModelInfo abs_model_info = {
    .model_absolute_path = "",
    .position = model_info.position,
    .orientation = model_info.orientation, 
    .scale = model_info.scale,
    .opacity = model_info.opacity,
  };

  // This ifdef SOURCE_PATH preprocessor directive is a cheat to silence VSCode Warnings
  // SOURCE_PATH is defined in the CMake build. THIS CODE WILL RUN!!!!
  // TODO: fix this. cause this will not work if the binary is moved off the machine
  #ifdef SOURCE_PATH
  switch (model_info.preset_type)
  {
    case PresetType::BOX:
    abs_model_info.model_absolute_path = std::string(SOURCE_PATH) + "example_models/UnitCube.stl";
    break;
    
    case PresetType::CYLINDER:
    abs_model_info.model_absolute_path = std::string(SOURCE_PATH) + "example_models/UnitCylinder.stl";
    break;
    
    case PresetType::SPHERE:
    abs_model_info.model_absolute_path = std::string(SOURCE_PATH) + "example_models/UnitSphere.stl";
    break;
    
    default:
    return;
  }
  #endif
  
  add_model_queue.push(abs_model_info);
}