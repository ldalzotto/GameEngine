#pragma once

#include "glm/glm.hpp"
#include "vulkan/vulkan.h"

#include "Render/Memory/VulkanBuffer.h"

#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2;
	struct Device;
	struct TextureResourceProvider;
	struct MeshResourceProvider;
	struct MeshResourceProviderUseResourceInfo;
	struct Mesh;
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

	struct DefaultMaterialV2DrawerResourceProviderDependencies
	{
		TextureResourceProvider* TextureResourceProvider;
		MeshResourceProvider* MeshResourceProvider;
	};

	struct DefaultMaterialV2DrawerAllocInfo
	{
		DefaultMaterialV2* DefaultMaterial;
		MeshResourceProviderUseResourceInfo* MeshResourceProviderUseResourceInfo;
		TextureUniqueKey TextureUniqueKey;
		DefaultMaterialV2DrawerResourceProviderDependencies* ResourceProviderDependencies;
		Device* Device;
	};

	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

	void DefaultMaterialV2Instance_alloc(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, DefaultMaterialV2DrawerAllocInfo* p_defaultMaterialV2InstanceAllocInfo);

	struct DefaultMaterialV2InstanceFreeInfo
	{
		Device* Device;
		DefaultMaterialV2DrawerResourceProviderDependencies* ResourceProviderDependencies;
	};

	void DefaultMaterialV2Instance_free(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, DefaultMaterialV2InstanceFreeInfo* p_defaultMaterialFreeInfo);

	void DefaultMaterialV2Instance_setModelMatrix(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, ModelProjection* p_modelProjection, Device* p_device);
};