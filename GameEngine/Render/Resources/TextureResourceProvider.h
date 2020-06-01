#pragma once

#include <unordered_map>
#include "Utils/Resource/UsageCounter.h"
#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
}

namespace _GameEngine::_Render
{
	struct ResourceWithCounter
	{
		Texture* Texture;
		_Utils::UsageCounter UsageCounter;
	};

	struct TextureResourceProviderDependencies
	{
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	struct TextureResourceProvider
	{
		TextureResourceProviderDependencies TextureResourceProviderDependencies;
		std::unordered_map<size_t, ResourceWithCounter> TextureResources;
	};

	void TextureResourceProvider_Clear(TextureResourceProvider* p_textureResourceProvider);
	Texture* TextureResourceProvider_UseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key);
	void TextureResourceProvider_ReleaseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key);
}