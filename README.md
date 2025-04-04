# sdformat-editor

SDFormat Editor is developed by Zaid Duraid, Evan Vokey, and Ean Wheeler for the ECE5100: Software Design Project at Memorial University of Newfoundland.

## Project Description

This project is a stand-alone GUI editor for SDF (Simulation Description Format) files, which are primarily used by the Gazebo robotics simulation tool. The editor provides a 3D viewer alongside an for editing models, links, joints, and other simulation components defined by the SDF specification.

The project is built in C++ using the CMake build system. it depends on OGRE for 3D rendering, `libsdformat14` for SDF parsing, and Dear ImGui for the user interface.

## Build Enviroment / OS Support

Development and use of this application is only supported on Ubuntu 24.04 LTS.

To build the project on your own machine, you must first add the OSRFoundation Gazebo Harmonic APT repository to your machine.

```
sudo apt-get update
sudo apt-get install curl lsb-release gnupg

sudo curl https://packages.osrfoundation.org/gazebo.gpg --output /usr/share/keyrings/pkgs-osrf-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/pkgs-osrf-archive-keyring.gpg] http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/gazebo-stable.list > /dev/null
sudo apt-get update
```

Next, ensure the following project dependancies are installed:

```
sudo apt-get install build-essential git cmake libglfw3-dev libgl1-mesa-dev libx11-dev zenity libsdformat14-dev
```

Clone the project. Since the project uses git submodules, use the `--recursive`

```
git clone --recursive https://github.com/sdformat-editor/sdformat-editor
```

and build the project with CMake and run:

```
mkdir build
cd build

cmake ..

make

./sdformat-editor
```

Your first build can take a while to complete as most dependancies are built from source. You can speed up the build by using `make -j$(nproc)` instead of `make`.