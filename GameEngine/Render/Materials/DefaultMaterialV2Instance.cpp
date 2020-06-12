#include "DefaultMaterialV2Instance.h"

#include "RenderInterface.h"

#include <stdexcept>
#include "Log/Log.h"

#include "DefaultMaterialV2.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "MaterialInstanceKeys.h"

#include "LoopStep/DefaultMaterialDrawStep.h"

#include "Mesh/Mesh.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	void createDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);
	void updateDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);
	void freeDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device);

	void DefaultMaterialV2Instance_alloc(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface, DefaultMaterialV2DrawerAllocInfo* p_defaultMaterialV2InstanceAllocInfo)
	{
		{
			p_defaultMaterialV2Instance->_DefaultMaterial = p_defaultMaterialV2InstanceAllocInfo->DefaultMaterial;
		}

		{
			MaterialInstance_init(&p_defaultMaterialV2Instance->MaterialInstance, p_renderInterface);
		}

		{
			MeshUniqueKey l_meshKey{};
			l_meshKey.MeshAssetPath = std::string((char*)p_defaultMaterialV2InstanceAllocInfo->InputParameters.at(MATERIALINSTANCE_MESH_KEY));
			MaterialInstance_setMesh(&p_defaultMaterialV2Instance->MaterialInstance, MATERIALINSTANCE_MESH_KEY, &l_meshKey);
		}

		for (ShaderParameter& l_shaderParameter : p_defaultMaterialV2InstanceAllocInfo->DefaultMaterial->LocalInputParameters.ShaderParameters)
		{
			switch (l_shaderParameter.Type)
			{
			case ShaderParameterType::UNIFORM_BUFFER:
			{
				UniformBufferParameter* l_uniformBufferParameter = (UniformBufferParameter*)l_shaderParameter.Parameter;
				MaterialInstance_setUniformBuffer(&p_defaultMaterialV2Instance->MaterialInstance, l_shaderParameter.KeyName, l_uniformBufferParameter);
			}
			break;
			case ShaderParameterType::IMAGE_SAMPLER:
			{
				ImageSampleParameter* l_imageSamplerParameter = (ImageSampleParameter*)l_shaderParameter.Parameter;
				TextureUniqueKey l_textureUniqueKey{};
				l_textureUniqueKey.TexturePath = std::string((char*)p_defaultMaterialV2InstanceAllocInfo->InputParameters.at(l_shaderParameter.KeyName));
				MaterialInstance_setTexture(&p_defaultMaterialV2Instance->MaterialInstance, l_shaderParameter.KeyName, &l_textureUniqueKey);
			}
			break;
			}
		}

		createDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
		updateDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
	};

	void DefaultMaterialV2Instance_free(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, RenderInterface* p_renderInterface)
	{
		{
			MaterialInstance_free(&p_defaultMaterialV2Instance->MaterialInstance);
		}
		freeDescriptorSet(p_defaultMaterialV2Instance, p_renderInterface->Device);
	};

	void updateDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		std::vector<VkWriteDescriptorSet> l_writeDescirptorSets;
		std::vector<VkDescriptorBufferInfo> l_descriptorBufferInfo(p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.ShaderParameters.size());
		std::vector<VkDescriptorImageInfo> l_descriptorImageInfo(p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.ShaderParameters.size());

		size_t i = 0;
		for (ShaderParameter& l_shaderParameter : p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.ShaderParameters)
		{
			switch (l_shaderParameter.Type)
			{
			case ShaderParameterType::UNIFORM_BUFFER:
			{
				UniformBufferParameter* l_uniformBufferParameter = (UniformBufferParameter*)l_shaderParameter.Parameter;

				VkWriteDescriptorSet l_writeDescriptorSet = UniformBufferParameter_buildWriteDescriptorSet(
					l_uniformBufferParameter,
					MaterialInstance_getUniformBuffer(&p_defaultMaterialV2Instance->MaterialInstance, l_shaderParameter.KeyName),
					&l_descriptorBufferInfo.at(i),
					p_defaultMaterialV2Instance->MaterialDescriptorSet);
				l_writeDescirptorSets.emplace_back(l_writeDescriptorSet);
			}
			break;
			case ShaderParameterType::IMAGE_SAMPLER:
			{
				ImageSampleParameter* l_imageSamplerParameter = (ImageSampleParameter*)l_shaderParameter.Parameter;

				VkWriteDescriptorSet l_writeDescriptorSet = ImageSampleParameter_buildWriteDescriptorSet(
					l_imageSamplerParameter,
					MaterialInstance_getTexture(&p_defaultMaterialV2Instance->MaterialInstance, l_shaderParameter.KeyName),
					&l_descriptorImageInfo.at(i),
					p_defaultMaterialV2Instance->MaterialDescriptorSet);
				l_writeDescirptorSets.emplace_back(l_writeDescriptorSet);
			}
			break;
			}

			i += 1;
		}

		vkUpdateDescriptorSets(p_device->LogicalDevice.LogicalDevice, l_writeDescirptorSets.size(), l_writeDescirptorSets.data(), 0, nullptr);
	};

	void createDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorSetLayout.DescriptorSetLayout;
		if (vkAllocateDescriptorSets(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_defaultMaterialV2Instance->MaterialDescriptorSet) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create description set."));
		};
	};

	void freeDescriptorSet(DefaultMaterialV2Instance* p_defaultMaterialV2Instance, Device* p_device)
	{
		vkFreeDescriptorSets(p_device->LogicalDevice.LogicalDevice, p_defaultMaterialV2Instance->_DefaultMaterial->LocalInputParameters.DescriptorPool.DescriptorPool, 1, &p_defaultMaterialV2Instance->MaterialDescriptorSet);
	};
};