#pragma once

#include <unordered_map>
#include "Utils/Resource/UsageCounter.h"
#include "Render/Mesh/Mesh.h"

namespace _GameEngine::_Render
{
	struct MeshResourceWithCounter
	{
		Mesh* Mesh;
		_Utils::UsageCounter UsageCounter;
	};

	struct MeshResourceProviderDependencies
	{
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	struct MeshResourceProvider
	{
		MeshResourceProviderDependencies MeshResourceProviderDependencies;
		std::unordered_map<size_t, MeshResourceWithCounter> MeshResources;
	};

	void MeshResourceProvider_Clear(MeshResourceProvider* p_meshResourceProvider);
	Mesh* MeshResourceProvider_UseResource(MeshResourceProvider* p_meshResourceProvider, MeshUniqueKey* p_key);
	void MeshResourceProvider_ReleaseResource(MeshResourceProvider* p_meshResourceProvider, MeshUniqueKey* p_key);
}