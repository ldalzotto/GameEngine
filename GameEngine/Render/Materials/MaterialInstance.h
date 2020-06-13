#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include "DataStructures/VectorT.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "Shader/ShaderParameter.h"

namespace _GameEngine::_Render
{
	struct Mesh;
	struct MeshUniqueKey;
	struct Texture;
	struct TextureUniqueKey;
	struct UniformBufferParameter;
	struct RenderInterface;
	struct DescriptorSetLayout;
	struct DescriptorPool;
	struct Material;
}

namespace _GameEngine::_Render
{
	struct MaterialInstanceParameter
	{
		size_t Key;
		void(*FreeCallback)(MaterialInstanceParameter*, RenderInterface*);
		void* Parameter;
	};

	void MaterialInstanceParameter_free(MaterialInstanceParameter** p_materialInstanceParameter, RenderInterface* p_renderInterface);

	struct MaterialInstance
	{
		RenderInterface* RenderInterface;
		Material* SourceMaterial;

		VkDescriptorSet MaterialDescriptorSet;
		_Core::VectorT<MaterialInstanceParameter*> Parameters;
	};

	//TODO This structure must be replaced with a generic Material object.
	struct MaterialInstanceInitInfo
	{
		Material* SourceMaterial;
		std::unordered_map<std::string, void*> MaterialInstanceInputParameters;
	};

	void MaterialInstance_init(MaterialInstance* p_materialInstance, RenderInterface* p_renderInterface, MaterialInstanceInitInfo* p_materialInstanceInitInfo);
	void MaterialInstance_free(MaterialInstance* p_materialInstance);

	struct MeshMaterialInstanceParameter
	{
		Mesh* Mesh;
	};

	void MeshMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, MeshUniqueKey* p_meshUniqueKey);
	Mesh* MaterialInstance_getMesh(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setMesh(MaterialInstance* p_materialInstance, std::string& p_key, MeshUniqueKey* p_meshUniqueKey);

	struct TextureMaterialInstanceParameter
	{
		Texture* Texture;
	};

	void TextureMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, TextureUniqueKey* p_textureUniqueKey);
	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, std::string& p_key, TextureUniqueKey* p_textureKey);

	struct UniformBufferInstanceParameter
	{
		VulkanBuffer UniformBuffer;
	};

	void UniformBufferInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, UniformBufferParameter* p_uniformBufferParameter);
	VulkanBuffer* MaterialInstance_getUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key, UniformBufferParameter* p_uniformBufferParameter);
	void MaterialInstance_pushUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key, void* p_data);
}