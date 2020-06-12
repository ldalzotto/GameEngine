#pragma once

#include <string>
#include "DataStructures/VectorT.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct MeshUniqueKey;
	struct Texture;
	struct RenderInterface;
	struct Mesh;
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
	
	/*
	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, std::string& p_key);
	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, std::string& p_key, Texture* p_texture);
	*/

	struct MeshMaterialInstanceParameter
	{
		Mesh* Mesh;
	};

	void MeshMaterialInstanceParameter_alloc(MaterialInstanceParameter* l_parent, RenderInterface* p_renderInterface, MeshUniqueKey* p_meshUniqueKey);

	struct TextureMaterialInstanceParameter
	{
		Texture* Texture;
	};

	struct ModelMatrixInstanceParameter
	{
		VulkanBuffer ModelMatrixBuffer;
	};

}