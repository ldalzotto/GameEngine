# GameEngine

Supported platform : Windows only.

![](https://i.imgur.com/BIZjD6j.gif)
[![](https://i.imgur.com/GxjQt6Q.png)](https://imgur.com/a/RKVNkiz)

## External dependencies

The Game Engine doesn't have any external dependencies.

## Milestone 0.0.3 features

This milestone added the following features :
1. [Software Rendering](https://github.com/ldalzotto/GameEngine/tree/b_0.0.3/Render) : A new software renderer prototype has been created from scratch. It isn't optimized at all.
2. <b>Code migration to C</b> : The source code has been migrated to the C language. That's because I was only using C++ template feature.
3. [Relative Assetpath](https://github.com/ldalzotto/GameEngine/tree/b_0.0.3/Core_C/Asset) : External assets must now be placed in the "Assets" folder at the same level as the executable.

### Software Renderer
The new software renderer has been created from scratch and replaces Vulkan. My goal here is to learn things about rendering technique, I am not interreseted in using premade library even if performance is worse.
The renderer features :
1/ Gizmo rendering.
2/ 3D Polygon rendering.
3/ Basic direction light shading per polygon.
4/ Asset loading and material instance.

 
