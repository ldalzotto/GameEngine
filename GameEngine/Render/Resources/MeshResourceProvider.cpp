#include "MeshResourceProvider.h"

#include "MyLog/MyLog.h"

#include "RenderInterface.h"
#include "Mesh/MeshLoader.h"

namespace _GameEngine::_Render
{

	Mesh* MeshResourceProvider_UseResource(MeshResourceProvider* p_meshResourceProvider, MeshResourceProviderUseResourceInfo* p_meshResourceProviderResourceInfo)
	{
		MeshUniqueKey l_meshUniqueKey{};
		l_meshUniqueKey.MeshAssetPath = p_meshResourceProviderResourceInfo->Meshpath;
		size_t l_hash = _CoreV3::Hash(&l_meshUniqueKey);

		if (!p_meshResourceProvider->MeshResources.contains(l_hash))
		{
			std::vector<_Render::Vertex> l_vertices = {};
			std::vector<uint16_t> l_indices = {};

			ReadMeshFromFileInfo l_readMeshFromFileInfo{};
			l_readMeshFromFileInfo.Path = p_meshResourceProviderResourceInfo->Meshpath;
			l_readMeshFromFileInfo.Vertices = &l_vertices;
			l_readMeshFromFileInfo.Indices = &l_indices;
			MeshLoader_readMeshFromFile(&l_readMeshFromFileInfo);

			MeshAllocProceduralInfo l_meshAllocInfo{};
			l_meshAllocInfo.MeshUniqueKey = &l_meshUniqueKey;
			l_meshAllocInfo.Device = p_meshResourceProvider->RenderInterface->Device;
			l_meshAllocInfo.PreRenderDeferedCommandBufferStep = p_meshResourceProvider->RenderInterface->PreRenderDeferedCommandBufferStep;
			l_meshAllocInfo.Vertices = &l_vertices;
			l_meshAllocInfo.Indices = &l_indices;
		
			Mesh* l_mesh = Mesh_allocProcedural(&l_meshAllocInfo);
			MeshResourceWithCounter l_resouceWithCounter{};

			l_resouceWithCounter.Mesh = l_mesh;
			l_resouceWithCounter.UsageCounter.UsageCount = 0;
			p_meshResourceProvider->MeshResources.emplace(l_hash, l_resouceWithCounter);
		}

		MeshResourceWithCounter* l_resourceWithCounter = &p_meshResourceProvider->MeshResources.at(l_hash);
		_Utils::UsageCounter_use(&l_resourceWithCounter->UsageCounter);
		return l_resourceWithCounter->Mesh;
	};

	void MeshResourceProvider_ReleaseResource(MeshResourceProvider* p_meshResourceProvider, MeshUniqueKey* p_key)
	{
		size_t l_hash = _CoreV3::Hash(p_key);
		MeshResourceWithCounter* l_resourceWithCounter = &p_meshResourceProvider->MeshResources.at(l_hash);
		_Utils::UsageCounter_release(&l_resourceWithCounter->UsageCounter);
		if (l_resourceWithCounter->UsageCounter.UsageCount == 0)
		{
			Mesh_free(&l_resourceWithCounter->Mesh, p_meshResourceProvider->RenderInterface->Device);
			p_meshResourceProvider->MeshResources.erase(l_hash);
		}
	};

	void MeshResourceProvider_Clear(MeshResourceProvider* p_meshResourceProvider)
	{
		if (p_meshResourceProvider->MeshResources.size() > 0)
		{
#ifndef NDEBUG
			std::string l_textureResourcesNotDisposed = "[";
			for (auto&& l_textureResourceEntry : p_meshResourceProvider->MeshResources)
			{
				l_textureResourcesNotDisposed += l_textureResourceEntry.second.Mesh->MeshUniqueKey.MeshAssetPath;
				l_textureResourcesNotDisposed += ",";
			}
			l_textureResourcesNotDisposed += "]";
			MYLOG_PUSH(p_meshResourceProvider->RenderInterface->MyLog, _Log::WARN, (char*)("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.").c_str());
#endif

			for (auto&& l_textureResourceEntry : p_meshResourceProvider->MeshResources)
			{
				Mesh_free(&l_textureResourceEntry.second.Mesh, p_meshResourceProvider->RenderInterface->Device);
			}
			p_meshResourceProvider->MeshResources.clear();
		}
	};
}