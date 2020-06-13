#pragma once

#include <unordered_map>
#include "Materials/Material.h"
#include "Utils/Resource/UsageCounter.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct MaterialWithCounter
	{
		Material* Material;
		_Utils::UsageCounter UsageCounter;
	};

	struct MaterialResourceProvider
	{
		RenderInterface* RenderInterface;
		std::unordered_map<size_t, MaterialWithCounter> MaterialResources;
	};

	void MaterialResourceProvider_Clear(MaterialResourceProvider* p_materialResourceProvider);
	Material* MaterialResourceProvider_UseResource(MaterialResourceProvider* p_materialResourceProvider, MaterialUniqueKey* p_key);
	void MaterialResourceProvider_ReleaseResource(MaterialResourceProvider* p_materialResourceProvider, MaterialUniqueKey* p_key);
}