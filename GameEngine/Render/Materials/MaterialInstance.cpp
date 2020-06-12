#include "MaterialInstance.h"

#include <functional>

#include "RenderInterface.h"
#include "Resources/MeshResourceProvider.h"
#include "Resources/TextureResourceProvider.h"
#include "Mesh/Mesh.h"
#include "Shader/ShaderParameter.h"

namespace _GameEngine::_Render
{
	bool MaterialInstanceParameter_vectorFind(MaterialInstanceParameter** left, size_t* right)
	{
		return (*left)->Key == *right;
	};

	void MeshMaterialInstanceParameter_free(MaterialInstanceParameter* p_materialInstanceParameter, RenderInterface* p_renderInterface);

	void MaterialInstance_init(MaterialInstance* p_materialInstance, RenderInterface* p_renderInterface)
	{
		p_materialInstance->RenderInterface = p_renderInterface;
		p_materialInstance->Parameters.alloc(4);
	};

	void MaterialInstance_free(MaterialInstance* p_materialInstance)
	{
		for (size_t i = 0; i < p_materialInstance->Parameters.size(); i++)
		{
			MaterialInstanceParameter* l_materialInstanceParameter = *p_materialInstance->Parameters.at(i);
			MaterialInstanceParameter_free(&l_materialInstanceParameter, p_materialInstance->RenderInterface);
		}

		p_materialInstance->Parameters.free();
	};

	void MaterialInstanceParameter_free(MaterialInstanceParameter** p_materialInstanceParameter, RenderInterface* p_renderInterface)
	{
		MaterialInstanceParameter* l_materialInstanceParameter = *p_materialInstanceParameter;
		if (l_materialInstanceParameter->FreeCallback)
		{
			l_materialInstanceParameter->FreeCallback(l_materialInstanceParameter, p_renderInterface);
		}

		free(l_materialInstanceParameter->Parameter);
		l_materialInstanceParameter->Parameter = nullptr;

		free(l_materialInstanceParameter);
	};

	Mesh* MaterialInstance_getMesh(MaterialInstance* p_materialInstance, std::string& p_key)
	{
		size_t l_key = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundMaterialInstanceParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_key);
		if (l_foundMaterialInstanceParameter)
		{
			MaterialInstanceParameter* l_materialInstanceParameter = *l_foundMaterialInstanceParameter;
			MeshMaterialInstanceParameter* l_mesMaterialInstanceParameter = (MeshMaterialInstanceParameter*)l_materialInstanceParameter->Parameter;
			return l_mesMaterialInstanceParameter->Mesh;
		}

		return nullptr;
	};

	void MaterialInstance_setMesh(MaterialInstance* p_materialInstance, std::string& p_key, MeshUniqueKey* p_meshUniqueKey)
	{
		size_t l_key = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundMaterialInstanceParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_key);
		if (l_foundMaterialInstanceParameter)
		{
			MaterialInstanceParameter_free(l_foundMaterialInstanceParameter, p_materialInstance->RenderInterface);
			p_materialInstance->Parameters.erase(MaterialInstanceParameter_vectorFind, &l_key);
		}

		MaterialInstanceParameter* l_materialInstanceParameter = (MaterialInstanceParameter*)calloc(1, sizeof(MaterialInstanceParameter));
		MeshMaterialInstanceParameter_alloc(l_materialInstanceParameter, p_materialInstance->RenderInterface, p_meshUniqueKey);
		l_materialInstanceParameter->Key = l_key;
		p_materialInstance->Parameters.push_back(&l_materialInstanceParameter);
	};

	void MeshMaterialInstanceParameter_free(MaterialInstanceParameter* p_materialInstanceParameter, RenderInterface* p_renderInterface)
	{
		MeshMaterialInstanceParameter* l_meshparameter = (MeshMaterialInstanceParameter*)p_materialInstanceParameter->Parameter;
		MeshResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.MeshResourceProvider, &l_meshparameter->Mesh->MeshUniqueKey);
		l_meshparameter->Mesh = nullptr;
	};

	void MeshMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, MeshUniqueKey* p_meshUniqueKey)
	{
		MeshMaterialInstanceParameter* l_meshMaterialInstanceParameter = (MeshMaterialInstanceParameter*)calloc(1, sizeof(MeshMaterialInstanceParameter));

		l_parent->Parameter = l_meshMaterialInstanceParameter;
		l_parent->FreeCallback = MeshMaterialInstanceParameter_free;

		MeshResourceProviderUseResourceInfo l_meshResourceUseInfo{};
		l_meshResourceUseInfo.Meshpath = p_meshUniqueKey->MeshAssetPath;
		l_meshMaterialInstanceParameter->Mesh = MeshResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.MeshResourceProvider, &l_meshResourceUseInfo);
	};


	void TextureMaterialInstanceParameter_free(MaterialInstanceParameter* p_materialInstanceParameter, RenderInterface* p_renderInterface)
	{
		TextureMaterialInstanceParameter* l_textureParameter = (TextureMaterialInstanceParameter*)p_materialInstanceParameter->Parameter;
		TextureResourceProvider_ReleaseResource(p_renderInterface->ResourceProvidersInterface.TextureResourceProvider, &l_textureParameter->Texture->TextureUniqueKey);
		l_textureParameter->Texture = nullptr;
	};

	void TextureMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, TextureUniqueKey* p_textureUniqueKey)
	{
		TextureMaterialInstanceParameter* l_textureMaterialInstanceParameter = (TextureMaterialInstanceParameter*)calloc(1, sizeof(TextureMaterialInstanceParameter));

		l_parent->Parameter = l_textureMaterialInstanceParameter;
		l_parent->FreeCallback = TextureMaterialInstanceParameter_free;

		l_textureMaterialInstanceParameter->Texture = TextureResourceProvider_UseResource(p_renderInterface->ResourceProvidersInterface.TextureResourceProvider, p_textureUniqueKey);
	};

	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, std::string& p_key)
	{
		size_t l_hash = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_hash);
		if (l_foundParameter)
		{
			return ((TextureMaterialInstanceParameter*)(*l_foundParameter)->Parameter)->Texture;
		}
		return nullptr;
	};

	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, std::string& p_key, TextureUniqueKey* p_textureKey)
	{
		size_t l_hash = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_hash);
		if (l_foundParameter)
		{
			MaterialInstanceParameter_free(l_foundParameter, p_materialInstance->RenderInterface);
			p_materialInstance->Parameters.erase(MaterialInstanceParameter_vectorFind, &l_hash);
		}

		MaterialInstanceParameter* l_materialInstanceParameter = (MaterialInstanceParameter*)calloc(1, sizeof(MaterialInstanceParameter));
		TextureMaterialInstanceParameter_alloc(l_materialInstanceParameter, p_materialInstance->RenderInterface, p_textureKey);
		l_materialInstanceParameter->Key = l_hash;
		p_materialInstance->Parameters.push_back(&l_materialInstanceParameter);
	};

	
	void UniformBufferInstanceParameter_free(MaterialInstanceParameter* p_materialInstanceParameter, RenderInterface* p_renderInterface)
	{
		UniformBufferInstanceParameter* l_uniformBufferParameter = (UniformBufferInstanceParameter*)p_materialInstanceParameter->Parameter;
		VulkanBuffer_free(&l_uniformBufferParameter->UniformBuffer, p_renderInterface->Device);
	};

	void UniformBufferInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, UniformBufferParameter* p_uniformBufferParameter)
	{
		UniformBufferInstanceParameter* l_uniformBufferInstanceParameter = (UniformBufferInstanceParameter*)calloc(1, sizeof(UniformBufferInstanceParameter));

		l_parent->Parameter = l_uniformBufferInstanceParameter;
		l_parent->FreeCallback = UniformBufferInstanceParameter_free;

		l_uniformBufferInstanceParameter->UniformBuffer = UniformBufferParameter_allocVulkanBuffer(p_uniformBufferParameter, p_renderInterface->Device);
	};

	VulkanBuffer* MaterialInstance_getUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key)
	{
		size_t l_hash = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_hash);
		if (l_foundParameter)
		{
			return &((UniformBufferInstanceParameter*)(*l_foundParameter)->Parameter)->UniformBuffer;
		}
		return nullptr;
	};
	
	void MaterialInstance_setUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key, UniformBufferParameter* p_uniformBufferParameter)
	{
		size_t l_hash = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_hash);
		if (l_foundParameter)
		{
			MaterialInstanceParameter_free(l_foundParameter, p_materialInstance->RenderInterface);
			p_materialInstance->Parameters.erase(MaterialInstanceParameter_vectorFind, &l_hash);
		}

		MaterialInstanceParameter* l_materialInstanceParameter = (MaterialInstanceParameter*)calloc(1, sizeof(MaterialInstanceParameter));
		UniformBufferInstanceParameter_alloc(l_materialInstanceParameter, p_materialInstance->RenderInterface, p_uniformBufferParameter);
		l_materialInstanceParameter->Key = l_hash;
		p_materialInstance->Parameters.push_back(&l_materialInstanceParameter);
	};

	void MaterialInstance_pushUniformBuffer(MaterialInstance* p_materialInstance, std::string& p_key, void* p_data)
	{
		size_t l_hash = std::hash<std::string>()(p_key);
		MaterialInstanceParameter** l_foundParameter = p_materialInstance->Parameters.get(MaterialInstanceParameter_vectorFind, &l_hash);
		if (l_foundParameter)
		{
			UniformBufferInstanceParameter* l_uniformBufferParameter = (UniformBufferInstanceParameter*) (*l_foundParameter)->Parameter;
			VulkanBuffer_pushToGPU(&l_uniformBufferParameter->UniformBuffer, p_materialInstance->RenderInterface->Device, p_data, l_uniformBufferParameter->UniformBuffer.BufferAllocInfo.Size);
		}
	};
}