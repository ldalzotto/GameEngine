#include "EntityConfigurations.h"


char* CubeMeshRendererMaterialKey = "E:/GameProjects/GameEngine/Assets/Models/Cube.obj";
_ECS::MeshRendererInitInfo CubeMeshRendererInit = {};

void EntityConfiguration_init()
{
	CubeMeshRendererInit.MeshResourcePath = CubeMeshRendererMaterialKey;
};
