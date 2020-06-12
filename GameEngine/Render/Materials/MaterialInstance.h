#pragma once

#include <string>
#include "DataStructures/VectorT.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct Mesh;
	struct MeshUniqueKey;
	struct Texture;
	struct TextureUniqueKey;
	struct RenderInterface;
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
		_Core::VectorT<MaterialInstanceParameter*> Parameters;
	};

	void MaterialInstance_init(MaterialInstance* p_materialInstance, RenderInterface* p_renderInterface);
	void MaterialInstance_free(MaterialInstance* p_materialInstance);

	Mesh* MaterialInstance_getMesh(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setMesh(MaterialInstance* p_materialInstance, std::string& p_key, MeshUniqueKey* p_meshUniqueKey);
	
	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, std::string& p_key, TextureUniqueKey* p_textureKey);

	struct MeshMaterialInstanceParameter
	{
		Mesh* Mesh;
	};

	void MeshMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, MeshUniqueKey* p_meshUniqueKey);

	struct TextureMaterialInstanceParameter
	{
		Texture* Texture;
	};

	void TextureMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, TextureUniqueKey* p_textureUniqueKey);

	struct ModelMatrixInstanceParameter
	{
		VulkanBuffer ModelMatrixBuffer;
	};

}