# GameEngine

![](https://i.imgur.com/GeH4koY.gif)

## External dependencies

The Game Engine has the following external dependencies :
* [Vulkan](https://www.khronos.org/vulkan/) <br/>
  For presenting and 3D rendering.
* [GLFW](https://github.com/glfw/glfw) <br/>
  For managing application window and user input.
* [Dear Imgui](https://github.com/ocornut/imgui) <br/>
  For editor GUI.
* [Assimp](https://github.com/assimp/assimp) <br/>
  For loading .obj scene files.
* [StbImage](https://github.com/nothings/stb) <br/>
  For loading images.

## Milestone 0.0.2 features

This milestone added the following features :
1. [`Core library`](https://github.com/ldalzotto/GameEngine/tree/master/Core_cpp) <br/>
   Library for handling data structures in a generic way has been heavily refactored.
2. [`Math library`](https://github.com/ldalzotto/GameEngine/tree/master/Math/v2) <br/>
   The Math library has also been heavily refactored to work with vector and matrix in the most generic way possible. It also clarified Qaternions calculatations (that was just copy/pasted before).
3. [`Manipulating Entity in the world`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/Editor/ObjectSelection) <br/>
   Adding the possibility to move/rotate/scale Entities in the world.
4. [`Platform specific clock and file reading`](https://github.com/ldalzotto/GameEngine/blob/master/Core_cpp/Read/File/File.cpp) <br/>
   Instead of using cpp provided library, we are using the platform specific ones (only Windows for now). This is a choice as in the future, window creation (and threads maybe ?) will be created without library.
