#include "MeshResourceProvider.h"

#include "Log/Log.h"

#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_Render
{

	size_t MeshUniqueKey_buildHash(MeshUniqueKey* p_key)
	{
		size_t l_hash = 0;
		_Utils::Hash_combine(l_hash, p_key->MeshAssetPath);
		return l_hash;
	};

	Mesh* MeshResourceProvider_UseResource(MeshResourceProvider* p_meshResourceProvider, MeshUniqueKey* p_key)
	{
		size_t l_hash = MeshUniqueKey_buildHash(p_key);

		if (!p_meshResourceProvider->MeshResources.contains(l_hash))
		{
			std::vector<_Render::Vertex> l_vertices = {
			    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
				{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
				{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
				{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
			};

			std::vector<uint16_t> l_indices = {
				 0, 1, 2, 2, 3, 0
			};


			MeshAllocProceduralInfo l_meshAllocInfo{};
			l_meshAllocInfo.MeshUniqueKey = p_key;
			l_meshAllocInfo.Device = p_meshResourceProvider->MeshResourceProviderDependencies.Device;
			l_meshAllocInfo.PreRenderDeferedCommandBufferStep = p_meshResourceProvider->MeshResourceProviderDependencies.PreRenderDeferedCommandBufferStep;
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
		size_t l_hash = MeshUniqueKey_buildHash(p_key);
		MeshResourceWithCounter* l_resourceWithCounter = &p_meshResourceProvider->MeshResources.at(l_hash);
		_Utils::UsageCounter_release(&l_resourceWithCounter->UsageCounter);
		if (l_resourceWithCounter->UsageCounter.UsageCount == 0)
		{
			Mesh_free(&l_resourceWithCounter->Mesh, p_meshResourceProvider->MeshResourceProviderDependencies.Device);
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
			_Log::LogInstance->CoreLogger->warn("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.");
#endif

			for (auto&& l_textureResourceEntry : p_meshResourceProvider->MeshResources)
			{
				Mesh_free(&l_textureResourceEntry.second.Mesh, p_meshResourceProvider->MeshResourceProviderDependencies.Device);
			}
			p_meshResourceProvider->MeshResources.clear();
		}
	};
}