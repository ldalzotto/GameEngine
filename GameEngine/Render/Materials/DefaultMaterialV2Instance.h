#pragma once

#include "glm/glm.hpp"
#include "vulkan/vulkan.h"

#include "Render/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2;
	struct Mesh;
	struct Texture;
	struct Device;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2Drawer_ExternalResources
	{
		Mesh* Mesh;
		Texture* Texture;
	};

	struct DefaultMaterialV2Instance
	{
		DefaultMaterialV2* _DefaultMaterial;

		DefaultMaterialV2Drawer_ExternalResources ExternalResources;
		VulkanBuffer ModelMatrixBuffer;
		VkDescriptorSet MaterialDescriptorSet;
	};

	struct DefaultMaterialV2DrawerAllocInfo
	{
		DefaultMaterialV2* DefaultMaterial;
		DefaultMaterialV2Drawer_ExternalResources* ExternalResource;
		Device* Device;
	};

	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

	void DefaultMaterialV2Instance_alloc(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, DefaultMaterialV2DrawerAllocInfo* p_defaultMaterialV2InstanceAllocInfo);
	void DefaultMaterialV2Instance_free(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);

	void DefaultMaterialV2Instance_setModelMatrix(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, ModelProjection* p_modelProjection, Device* p_device);
};