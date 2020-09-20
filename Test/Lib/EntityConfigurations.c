#include "EntityConfigurations.h"


char* CubeMeshRendererMaterialKey = "Models/Cube.obj";
MeshRendererInitInfo CubeMeshRendererInit = {0};

void EntityConfiguration_init()
{
	CubeMeshRendererInit.MeshResourcePath = CubeMeshRendererMaterialKey;
	CubeMeshRendererInit.Material.ShadingType = MATERIAL_SHADING_TYPE_FLAT;
	CubeMeshRendererInit.Material.BaseColor = (Color3f){ 0.5f, 0.5f, 0 };
	CubeMeshRendererInit.Material.MeshPropertyUsage = MATERIAL_MESHPROPERTY_USAGE_NONE;
};
