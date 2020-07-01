# GameEngine

This first milestone introduces the most essential features of what we can await of a Game Engine.

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

## Milestone 0.0.1 features

This milestone added the following features :
1. [`GameLoop`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/GameLoop) <br/>
   Monitoring calls betwwen the Input System, ECS System and Render engine.
2. [`RenderEngine`]() <br/>
   A Vulkan implementation that render 3D models.
3. [`ECS System`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/ECS) <br/>
   Used for ordering execution of systems within an Update call. All components and systems are [here](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/ECS_Impl).
4. [`Input System`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/Input) <br/>
   Input state of every key.
5. [`Transform hierarchy`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/ECS_Impl/Components/Transform/TransformComponent.h) <br/>
   Relations parent<->child between TransformComponent that allows parenting between objects.
6. [`Custom mathematic library`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/Common/Math) <br/>
   A custom math library that handles Vector, Matrices (and other) operations to move Transform in space and render them. Not the most clean code but hey 😋.
7. [`Custom data structures`](https://github.com/ldalzotto/GameEngine/tree/master/GameEngine/Common/DataStructures) <br/>
   The engine barely uses c++ standard library for data structures (if it does, then it's a mistake). Thus, custom container have been created.
   
## Render Engine features 

Because the Render Engine is a whole project on it's own, it's features are dissociated :
1. `Graphic concepts abstraction` <br/>
   Graphics objects that are common to all rendering engines have been abstracted to fit the Vulkan API. <br/>
   [`Texture`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Texture/Texture.h), 
   [`Mesh`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Mesh/Mesh.h), 
   [`Shader`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Shader/Shader.h),
   [`Material`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Materials/Material.h),
   [`MaterialInstance`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Materials/MaterialInstance.h), 
   [`Buffer`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/VulkanObjects/Memory/VulkanBuffer.h). 
2. [`Generic Materials`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Materials/MaterialInstance.h) <br/>
   Materials can have parameters (Vector4, Mesh, Textures, ...) setted in a generic way.
3. [`Gizmo Rendering`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Render/Gizmo/Gizmo.h) <br/>
   Rendering Gizmo in an immediate mode for debug rendering.
   
## Physics Engine features

The Physics Engine has just been born.
1. [`Physics World`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Physics/World/World.h) <br/>
   A very simple Physics World object that list all physics entities.
1. [`Raycasting`](https://github.com/ldalzotto/GameEngine/blob/master/GameEngine/Physics/World/RayCast.h) <br/>
   Raycasting against a vector of oriented box (OBB) is supported.
