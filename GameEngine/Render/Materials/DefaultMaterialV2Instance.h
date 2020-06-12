#pragma once

#include <string>

#include "glm/glm.hpp"
#include "vulkan/vulkan.h"
#include "MaterialInstance.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct DefaultMaterialV2;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2Instance
	{
		DefaultMaterialV2* _DefaultMaterial;

		MaterialInstance MaterialInstance;
		VulkanBuffer ModelMatrixBuffer;
		VkDescriptorSet MaterialDescriptorSet;
	};

	struct DefaultMaterialV2Instance_InputAssets
	{
		std::string MeshPath;
		std::string Texturepath;
	};

	struct DefaultMaterialV2DrawerAllocInfo
	{
		DefaultMaterialV2* DefaultMaterial;
		DefaultMaterialV2Instance_InputAssets* DefaultMaterialV2Instance_InputAssets;
	};

	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

	void DefaultMaterialV2Instance_alloc(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface, DefaultMaterialV2DrawerAllocInfo* p_defaultMaterialV2InstanceAllocInfo);

	void DefaultMaterialV2Instance_free(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface);

	void DefaultMaterialV2Instance_setModelMatrix(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface, ModelProjection* p_modelProjection);
};