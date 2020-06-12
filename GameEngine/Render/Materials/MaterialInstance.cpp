#include "MaterialInstance.h"

#include <functional>

#include "RenderInterface.h"
#include "Resources/MeshResourceProvider.h"
#include "Mesh/Mesh.h"

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

	/*
	Texture* MaterialInstance_getTexture(MaterialInstance* p_materialInstance, std::string& p_key)
	{
		if (p_materialInstance->Parameters.contains(p_key))
		{
			return (Texture*)p_materialInstance->Parameters.at(p_key);
		}
	};

	void MaterialInstance_setTexture(MaterialInstance* p_materialInstance, std::string& p_key, Texture* p_texture)
	{
		p_materialInstance->Parameters.emplace(p_key, p_texture);
	};
	*/
}