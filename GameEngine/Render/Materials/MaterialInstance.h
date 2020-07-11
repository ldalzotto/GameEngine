#pragma once

#include <unordered_map>
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
	struct MaterialInstanceParameter;

	//TODO - When allocated, the material instance immediately instanciate resources from ResrouceProvider.
	//       Thus, provoquing the allocation of any resources at potentially any places in the application. Polluting the execution flow of the application.
	//       MaterialInstance (and it's subsequent resource) can be allocated asynchronously.
	//       This can have benefits in the future where materials are instanciated in another thread. And remove the responsability of the MaterialInstance to load resources.
	struct MaterialInstance
	{
		RenderInterface* RenderInterface;
		Material* SourceMaterial;

		VkDescriptorSet MaterialDescriptorSet;
		_Core::VectorT<MaterialInstanceParameter*> Parameters;
	};

	typedef MaterialInstance* MaterialInstanceHandle;

	struct MaterialInstanceInitInfo
	{
		Material* SourceMaterial;
		std::unordered_map<ShaderParameterKey, void*> MaterialInstanceInputParameters;
	};

	MaterialInstance* MaterialInstance_alloc(RenderInterface* p_renderInterface, MaterialInstanceInitInfo* p_materialInstanceInitInfo);
	void MaterialInstance_free(MaterialInstance** p_materialInstance);




	struct MaterialInstanceParameter
	{
		size_t Key;
		void(*FreeCallback)(MaterialInstanceParameter*, RenderInterface*);
		void* Parameter;
	};

	void MaterialInstanceParameter_free(MaterialInstanceParameter** p_materialInstanceParameter, RenderInterface* p_renderInterface);


	struct MeshMaterialInstanceParameter
	{
		Mesh* Mesh;
	};

	void MeshMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, MeshUniqueKey* p_meshUniqueKey);
	Mesh* MaterialInstance_getMesh(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key);
	void MaterialInstance_setMesh(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key, MeshUniqueKey* p_meshUniqueKey);

	struct TextureMaterialInstanceParameter
	{
		Texture* Texture;
	};

	void TextureMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, TextureUniqueKey* p_textureUniqueKey);
	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key);
	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key, TextureUniqueKey* p_textureKey);

	struct UniformBufferInstanceParameter
	{
		VulkanBuffer UniformBuffer;
	};

	void UniformBufferInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, UniformBufferParameter* p_uniformBufferParameter);
	VulkanBuffer* MaterialInstance_getUniformBuffer(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key);
	void MaterialInstance_setUniformBuffer(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key, UniformBufferParameter* p_uniformBufferParameter);
	void MaterialInstance_pushUniformBuffer(MaterialInstance* p_materialInstance, ShaderParameterKey& p_key, void* p_data);
}