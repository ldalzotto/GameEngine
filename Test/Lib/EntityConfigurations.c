#include "EntityConfigurations.h"


char* CubeMeshRendererMaterialKey = "E:/GameProjects/GameEngine/Assets/Models/Cube.obj";
MeshRendererInitInfo CubeMeshRendererInit = {0};

void EntityConfiguration_init()
{
	CubeMeshRendererInit.MeshResourcePath = CubeMeshRendererMaterialKey;
	CubeMeshRendererInit.Material.BaseColor = (Vector3c){ 100, 100, 0 };
};
