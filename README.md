# sdformat-editor

## Development Environment Setup (Ubuntu / WSL)

```
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev libx11-dev
```

*Written in the words of Zaid*

I first suggested this idea as a space filler for our ECE 5010 cource project. Me, Evan, and Ean were pretty set on making an Inventory Tracker but we had to submit two ideas for the proposal. As I looked into this a little more, I realized that an SDF file editor may be kinda doable and, AFAIK RN, nobody has created one for the newest version of Gazebo. Also, the Gazebo Classic SDF file editor left a lot to be desired. 

The reason we are really considering this idea is because of Professor Andrew Vardy's comment on our proposal:
"AV: I may be biased, but the SDF file editor seems much more exciting and interesting than the inventory tracker."

## Initial Project Proposal

| Description            | A standalone visual SDF (simulation description format) file editor. The editor will focus on the SDF model feature and draw on the latest SDF [file specifications](http://sdformat.org/). It will include a 3D viewer for rendering models in real-time as they are being edited and a user interface that integrates the SDF file specifications and any user-specified custom simulation plugin and sensor attributes. Overall, the file editor will speed up workflow by allowing developers to visualize models quickly rather than having to relaunch full simulation instances. <br><br>Note: SDF files are used primarily in the Gazebo robotics simulator for defining worlds and robotic models. For robotic models, SDF format allows for defining links, joints, plugins, sensors, and more. |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| # Scope                | Features to be completed this term include an intuitive user interface that provides dropdowns, sliders, and text boxes depending on the field being edited. The user interface’s fields will be populated based on the SDF file format and user-specified custom simulation plugin and sensor attributes. The 3D viewer will allow for visualizing links and joints by appropriately loading and positioning 3D meshes (.stl, .dae, .obj, .bvh) and rerendering whenever the user makes changes. Out-of-scope features include a click-and-drag aspect to the 3D viewer and direct integration with the Gazebo robotics simulator.                                                                                                                                                                                 |
| # Programming Language | C++                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| # Platform             | Desktop application using GLFW + OpenGL for 3D rendering and Dear ImGUI for user interface                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |

### Initial Questions:
1. "It will include a 3D viewer for rendering models in real-time as they are being edited". **Why not make it a custom Gazebo World Plugin?** 
2. Read through and comment on SDF file specifications. **Do you see any problems? What XML parsing library would you use to write and parse XML? Will this be the same as how Gazebo parses SDF?**
3. **How would you store SDF specifications? Can you use the SDF API for this?** 
4. **What do you do when you discover a violation of XML specifications in a file?**
5. **How would the user be able to specify their custom plugins and sensors?**
6. **How would the workflow work? Do you have a "file" button up top that lets you open and edit models?** 
7. Depending on the answer to question 1 (which would probably be "no, we will not make it a plugin") look into OpenGL and ImGUI to see if there are any problems. Consider importing different file types. **Are there any problems?**
8. **What would it take to add click-and-drag? What would it take for direct integration with the Gazebo Robotics Simulator?**

## Research

The [GazeboSim Repos](https://github.com/orgs/gazebosim/repositories) include an SDFormat repo.

Will definitely use the Gazebo Harmonic [SDFormat](https://github.com/gazebosim/sdformat/tree/sdf14/examples) repo. They have an example for parsing...

I think I will stick to SDF 14 since it is the latest stable version (associated with the latest stable version of Gazebo, which is Harmonic). 

[This Tutorial](http://sdformat.org/tutorials?tut=quickstart&cat=developers&) actually goes over file parsing which is pretty nifty. 

The SDFormat repo is literally the API that is associated with the SDFormat website. They even have an example for parsing a model and finding all associated links and joints. 

![[SDF File Parsing.png]]


In the 3D viewer, we should render the XYZ coordinate axes for visualization of links and joints.

There is a [Python binding](http://sdformat.org/tutorials?tut=python_bindings&cat=developers&) that lets you add stuff like links and models, as well as setting attributes. You can naturally do this in C++ as well, but the Python binding conveniently has all the required features.

Ok I've built the SDFormat repo from source. 

## How this would actually be done

- Store a tree that starts with the SDF root element. 
- Under the root element, the editor would only allow for adding a model. Thus model sits under root in the tree.
	- **An attribute is defined in the element's tag, ex `<model attribute="model_attribute><\model>`**
	- **An element is defined under another element or the SDF root, ex `<model> <element><\element><\model>`**
- Under model, ALL possible elements and attributes have to be included in the tree. This is similar to the tree in [The SDFormat Website](http://sdformat.org/spec?ver=1.12&elem=model). 
- This would continue recursively for every single element. Some of the very large elements, such as `link` or `joint`, will link you to their own embedded trees in the SDFormat website
- If the tree on the SDFormat website can be turned into some parsable tree datastructure, that would make this whole project possible

Now the question is: How would the SDF file be edited? The API seems to already provide functionality to actually edit everything about an element (ex. `SetName()` method of `model`, which sets the name attribute). This leaves two options for modifying the SDF
- Link the "read" and "write" functionality of each attribute and element node in the tree with the associated method name. I think this would have to be done manually 
- Just store the `Required`, `Type`, and `Default`, and `Description` parameters of all the nodes defined on the SDFormat website and use that along with the tree structure to determine how to edit models. I think this would be the cleanest way.

### 3D Viewer

The UI would let you visualize and edit the tree. The 3D viewer will ignore everything in the tree except for `visual`, `collision`, and `include` elements. Those are the only places were you would have to render something to see. 
- This is good because you don't have to worry about sensors and custom plugins

You would also need to look for `pose` and `axis` elements of `links`, `joints`, and `frames`. When those elements are selected in the dropdown, there should be a `visualize` button. 

Finally, there should be a `rerender` button at the bottom of the editor to let the 3D viewer re-parse the tree to render all elements listed above where `visualize` is set to True.


## Verdict

There remains two large roadblocks. 
1. Being able to store the [SDFormat tree](http://sdformat.org/spec?ver=1.12&elem=sdf) as some sort of data structure. I need to find out how they generated it on their website
2. Finding out how hard it would be to render these 3D elements in the viewer. Note that through just parsing, we would have access to 1) position relative to model frame (including orientation) and 2) 3D model (.stl, .dae, whatever) or general shape (box, sphere, cylinder)

## SDFormat Tree

Ok so I found it.

If you clone their repo, you can do to `sdf/1.11` and find their schema.

After cloning their repo, I was able to do:
```
python3 xmlschema.py --input-file /home/zaid/Documents/sdformat/sdf/1.11/model.sdf --sdf-dir /home/zaid/Documents/sdformat/sdf/1.11/ --output-dir /home/zaid/Documents/sdformat/tools
```
This returned some sort of model in `.xsd` format. Probably not useful.

In any case, generating the tree data structure is doable 👍

#### Extra Note

If we are parsing a pre-created model, SDF format does have everything needed for this. In addition to specific API for each element, it seems that all elements are convertible into a Base class of sorts which is called `Element`. May need to look into this.

## 3D Viewer

Once we have our tree data structure figured out, we can get all the required information for the 3D viewer. 

AI had a go at this:

Since your **3D viewer is purely static**, the difficulty level is much more manageable. You only need to **parse the SDF file, load 3D models, apply transforms, and visualize joints with axis indicators**. No need for physics, animations, or real-time updates.

Here’s a breakdown of what’s involved and how challenging each step is:

---

## **1. Scope of Work (Easier than Before)**

You will need to:  
✅ **Load 3D models** (STL, DAE, OBJ, etc.)  
✅ **Position and orient them correctly** using transformation matrices  
✅ **Render basic geometric shapes** (box, sphere, cylinder) for simpler models  
✅ **Render joints as XYZ axes** (Red = X, Green = Y, Blue = Z)

💡 Since everything is **static**, you only need to render **once per file load**, which simplifies performance concerns.

---

## **2. Estimated Difficulty per Component**

|Task|Difficulty|Notes|
|---|---|---|
|**Parsing SDF File**|**Easy**|SDFormat API already handles parsing for you.|
|**Loading 3D Models**|**Moderate**|Assimp simplifies STL/DAE/OBJ importing, but some formats might need extra processing.|
|**Rendering in OpenGL**|**Moderate**|Basic OpenGL knowledge is needed (VAOs, VBOs, shaders).|
|**Applying Transformations**|**Moderate**|Requires converting position/orientation into a transformation matrix.|
|**Rendering Joints (XYZ Axis)**|**Easy**|Draw simple colored arrows at joint positions.|
|**Rendering Basic Shapes (Box, Sphere, Cylinder)**|**Easy**|Use built-in OpenGL primitives (or load meshes for them).|

Overall, this is **not the hardest part of your project**, but **it is one of the more time-consuming ones** if you are new to OpenGL.

---

## **3. Step-by-Step Plan**

### **Step 1: Setup Window & Rendering**

- Initialize **GLFW + OpenGL** (or optionally **GLAD** for modern OpenGL).
- Set up a simple **shader program** to render 3D objects.

### **Step 2: Load and Render Models**

- Use **Assimp** to load **STL, DAE, OBJ** files.
- Convert model data into **VAOs & VBOs** for OpenGL rendering.
- Display them in the viewport.

### **Step 3: Apply Transformations**

- Parse **position and orientation** from SDF.
- Convert to a **4x4 transformation matrix**.
- Use **glm::translate(), glm::rotate()** to place models in the correct position.

### **Step 4: Render Simple Shapes for Basic Models**

- If an SDF entry specifies a **box, sphere, or cylinder**, render a simple OpenGL primitive instead of loading a mesh.

### **Step 5: Display Joints with XYZ Axes**

- Parse **joint positions** from SDF.
- Draw simple **lines/arrows** using OpenGL **GL_LINES**.

---

## **4. Tools & Libraries**

|Component|Suggested Library|Why?|
|---|---|---|
|**Windowing**|GLFW|Lightweight, good OpenGL support.|
|**Model Loading**|Assimp|Handles STL, DAE, OBJ formats easily.|
|**Math (Transforms)**|GLM|Great for 3D math (matrices, vectors).|
|**UI (If Needed)**|Dear ImGui|Good for adding simple UI elements.|

---

## **5. Is This the Hardest Part?**

🔹 **No, this is not the hardest part of the project.**  
🔹 The most challenging step is likely setting up **OpenGL rendering + handling different model formats correctly**.  
🔹 However, because the viewer is **static**, you avoid complex issues like physics, real-time updates, and performance bottlenecks.

**📌 If you’re comfortable with OpenGL, this is a moderate task. If you're new to OpenGL, expect a learning curve.**

Would you like guidance on setting up Assimp with OpenGL or handling transformations? 🚀