#include "EntityConfigurations.h"


_Render::MaterialUniqueKey CubeMeshRendererMaterialKey = {};
_ECS::MeshRendererInitInfo CubeMeshRendererInit = {};

void EntityConfiguration_init()
{
		std::unordered_map<std::string, void*> l_defaultMaterialInput
		{
			{_Render::MATERIALINSTANCE_MESH_KEY, "E:/GameProjects/GameEngine/Assets/Models/Cube.obj"},
			{_Render::MATERIALINSTANCE_TEXTURE_KEY, "E:/GameProjects/GameEngine/Assets/Textures/MinecraftDirtV2.png"}
		};

		CubeMeshRendererMaterialKey.VertexShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialVertex.spv";
		CubeMeshRendererMaterialKey.FragmentShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialFragment.spv";

		CubeMeshRendererInit.InputParameters = l_defaultMaterialInput;
		CubeMeshRendererInit.MaterialUniqueKey = &CubeMeshRendererMaterialKey;
};
