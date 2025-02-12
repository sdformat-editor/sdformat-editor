# sdformat-editor

## Development Environment Setup (Ubuntu / WSL)

```
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev libx11-dev
```

| Description            | A standalone visual SDF (simulation description format) file editor. The editor will focus on the SDF model feature and draw on the latest SDF [file specifications](http://sdformat.org/). It will include a 3D viewer for rendering models in real-time as they are being edited and a user interface that integrates the SDF file specifications and any user-specified custom simulation plugin and sensor attributes. Overall, the file editor will speed up workflow by allowing developers to visualize models quickly rather than having to relaunch full simulation instances. <br><br>Note: SDF files are used primarily in the Gazebo robotics simulator for defining worlds and robotic models. For robotic models, SDF format allows for defining links, joints, plugins, sensors, and more. |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| # Scope                | Features to be completed this term include an intuitive user interface that provides dropdowns, sliders, and text boxes depending on the field being edited. The user interface’s fields will be populated based on the SDF file format and user-specified custom simulation plugin and sensor attributes. The 3D viewer will allow for visualizing links and joints by appropriately loading and positioning 3D meshes (.stl, .dae, .obj, .bvh) and rerendering whenever the user makes changes. Out-of-scope features include a click-and-drag aspect to the 3D viewer and direct integration with the Gazebo robotics simulator.                                                                                                                                                                                 |
| # Programming Language | C++                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 |
| # Platform             | Desktop application using GLFW + OpenGL for 3D rendering and Dear ImGUI for user interface                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
## Research

The [GazeboSim Repos](https://github.com/orgs/gazebosim/repositories) include an SDFormat repo.

Will definitely use the Gazebo Harmonic [SDFormat](https://github.com/gazebosim/sdformat/tree/sdf14/examples) repo. They have an example for parsing...

Sick to SDF 14 since it is the latest stable version (associated with the latest stable version of Gazebo, which is Harmonic). 

[This Tutorial](http://sdformat.org/tutorials?tut=quickstart&cat=developers&) actually goes over file parsing which is pretty nifty. 

In the 3D viewer, we should render the XYZ coordinate axes for visualization of links and joints.

## How this would actually be done

- Store a tree that starts with the SDF root element. 
- Under the root element, the editor would only allow for adding a model. Thus model sits under root in the tree.
	- **An attribute is defined in the element's tag, ex `<model attribute="model_attribute><\model>`**
	- **An element is defined under another element or the SDF root, ex `<model> <element><\element><\model>`**
- Under model, ALL possible elements and attributes have to be included in the tree. This is similar to the tree in [The SDFormat Website](http://sdformat.org/spec?ver=1.12&elem=model). 
- This would continue recursively for every single element. Some of the very large elements, such as `link` or `joint`, will link you to their own embedded trees in the SDFormat website
- If the tree on the SDFormat website can be turned into some parsable tree datastructure, that would make this whole project possible

Now the question is: How would the SDF file be edited? 
- Link the "read" and "write" functionality of each attribute and element node in the tree with the associated method name. 
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

Go to `sdf/1.11` and to find the SDF schema.

In any case, generating the tree data structure is doable 👍

#### Extra Note

If we are parsing a pre-created model, SDFormat does have everything needed for this. In addition to specific API for each element, it seems that all elements are convertible into a Base class of sorts which is called `Element`. May need to look into this.

## 3D Viewer

Once we have our tree data structure figured out, we can get all the required information for the 3D viewer. 
Here’s a breakdown of what’s involved and how challenging each step is:

**Scope:**

1. Load 3D models (STL, DAE, OBJ, etc.)  
2. Position and orient them correctly using transformation matrices  
3. Render basic geometric shapes (box, sphere, cylinder) for simpler models  
4. Render joints as XYZ axes (Red = X, Green = Y, Blue = Z)

**Potential Tools & Libraries:**

|Component|Suggested Library|Why?|
|---|---|---|
|**Windowing**|GLFW|Lightweight, good OpenGL support.|
|**Model Loading**|Assimp|Handles STL, DAE, OBJ formats easily.|
|**Math (Transforms)**|GLM|Great for 3D math (matrices, vectors).|
|**UI (If Needed)**|Dear ImGui|Good for adding simple UI elements.|
