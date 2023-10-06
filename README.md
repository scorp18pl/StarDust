# Stardust

<h1 align="center">
  <img src="Media/stardust-logo-name.png" width="100%" alt="Universal">
</h1>

## Goals and Features

Stardust is a 2D and 3D C++ rendering engine built with OpenGL.
The direction of this project along with the API is subject to change.
Currently it is a hobby project and is not intended for production use.
The rendering engine is supposed to support low level and high level rendering.

## Installation prerequisites

The Stardust engine is built with CMake and requires installation of the following dependencies:

- [GLFW](https://www.glfw.org/)
- [OpenGL](https://www.opengl.org/)
- [Universal](https://github.com/scorp18pl/Universal)

The Stardust library also uses the following dependencies which are included in the repository (*under the
Code/3rdParty directory*):
 - [Glad](https://glad.dav1d.de/)
 - [ImGui](https://github.com/ocornut/imgui)

## Library installation

1. Clone the repository
```bash
git clone https://github.com/scorp18pl/StarDust.git
cd StarDust
```
2. Create a build directory and proceed to it
```bash
cd Code
mkdir build && cd build
```
3. Install the library with CMake
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
sudo make install
```

## Using the library (CMake)
In your project's CMakeLists.txt file add the following lines:
1. Find the library alongside the dependencies
```cmake
find_library(UNIVERSAL universal)
find_library(STARDUST stardust)
find_package(glfw3 REQUIRED)
```
2. Link the libraries to your project
```cmake
target_link_libraries(main PRIVATE ${STARDUST} ${UNIVERSAL} glfw)
```
