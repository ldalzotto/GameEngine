#include "EntityConfigurations.h"


char* CubeMeshRendererMaterialKey = "Models/Cube.obj";
MeshRendererInitInfo CubeMeshRendererInit = {0};

void EntityConfiguration_init()
{
	CubeMeshRendererInit.MeshResourcePath = CubeMeshRendererMaterialKey;
	CubeMeshRendererInit.Material.Type = MATERIAL_TYPE_FlatShaded_NotTextured;
	CubeMeshRendererInit.Material.BaseColor = (Color3f){ 0.5f, 0.5f, 0 };
};
