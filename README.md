# Welcome

The software renderer project is a 3D renderer created as a personal project and coded in C.

The software renderer program replicates computations done by the GPU hardware inside a C program.

This project was made to challenge myself to write a software rasterizer from scratch with little prior knowledge in C and 3D mathematics. Learning graphics concepts and math in the process. <br/>
It is a learning journey, so as much as possible, it will use handmade solutions (memory containers, maths
library, 3D scene management, 3D graphics abstraction, OS interactions).

# Features

* 3D mesh rasterization on the CPU.
* Line renderer.
* Pixel shading with texture mapping.
* Flat shading.
* 3D transformation gizmo to move entities in the world.
* obj file loader.

The big picture of the algorithms used for the software renderer and the entity movement are detailed here.

![](https://i.imgur.com/SPNdNuE.gif)
![](https://i.imgur.com/8LRG2xY.gif)

![](https://i.imgur.com/BIZjD6j.gif)

# Third party

Usage of third party libraries is limited to :

1. [stbimage](https://github.com/nothings/stb) for png loading.

## Entity selection

Click one of the cube to select it. Press T,R or S to enable translation, rotation and scale gizmos.

### Try it from precompiled binaries

Download and extract the [binaries](https://github.com/ldalzotto/SoftwareRasterizer/releases/download/0.0.3/GameEngine_0.0.3.7z).

### Or build from source

The project uses CMake as build system.

Clone the repository and on the root folder with power shell :

```
mkdir ./build
cd ./build
cmake ../
cmake --build . --target EntitySelectionTest
```

It will generate EntitySelectionTest.exe in the path mentioned in the command prompt.

## Software renderer shocase

### Try it from precompiled binaries

Download and extract the [binaries](https://github.com/ldalzotto/SoftwareRasterizer/releases/download/0.0.4/004_Release.7z).

### Or build from source

The project uses CMake as build system.

Clone the repository and on the root folder with power shell :

```
mkdir ./build
cd ./build
cmake ../
cmake --build . --target SoftwareRendererShowcase_icosphere
cmake --build . --target SoftwareRendererShowcase_tower
```

It will generate SoftwareRendererShowcase_icosphere.exe and SoftwareRendererShowcase_tower.exe in the path mentioned in the command prompt.

> **WARNING:** if you want to run the program from source, make sure to update the EXECUTABLEPATH_DEV definition in the source code to make it point to the project root path that contains the asset folder.
