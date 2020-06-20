#pragma once

#include <unordered_map>
#include <string>
#include "Utils/Resource/UsageCounter.h"
#include "Mesh/Mesh.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

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
		RenderInterface* RenderInterface;
		std::unordered_map<size_t, MeshResourceWithCounter> MeshResources;
	};

	void MeshResourceProvider_Clear(MeshResourceProvider* p_meshResourceProvider);

	struct MeshResourceProviderUseResourceInfo
	{
		std::string Meshpath;
	};

	Mesh* MeshResourceProvider_UseResource(MeshResourceProvider* p_meshResourceProvider, MeshResourceProviderUseResourceInfo* p_meshResourceProviderResourceInfo);
	void MeshResourceProvider_ReleaseResource(MeshResourceProvider* p_meshResourceProvider, MeshUniqueKey* p_key);
}