#include "EntityConfigurations.h"


char* CubeMeshRendererMaterialKey = "E:/GameProjects/GameEngine/Assets/Models/Cube.obj";
MeshRendererInitInfo CubeMeshRendererInit = {};

void EntityConfiguration_init()
{
	CubeMeshRendererInit.MeshResourcePath = CubeMeshRendererMaterialKey;
};
