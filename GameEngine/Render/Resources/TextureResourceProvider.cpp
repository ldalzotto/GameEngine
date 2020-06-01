#include "TextureResourceProvider.h"

#include "Log/Log.h"

#include "Utils/Algorithm/Algorithm.h"

namespace _GameEngine::_Render
{

	size_t TextureUniqueKey_buildHash(TextureUniqueKey* p_textureUniqueKey)
	{
		size_t l_hash = 0;
		_Utils::Hash_combine(l_hash, p_textureUniqueKey->TexturePath);
		return l_hash;
	};

	Texture* TextureResourceProvider_UseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key)
	{
		size_t l_hash = TextureUniqueKey_buildHash(p_key);

		if (!p_textureResourceProvider->TextureResources.contains(l_hash))
		{
			TextureLoadInfo l_textureLoadInfo{};
			l_textureLoadInfo.Device = p_textureResourceProvider->TextureResourceProviderDependencies.Device;
			l_textureLoadInfo.PreRenderDeferedCommandBufferStep = p_textureResourceProvider->TextureResourceProviderDependencies.PreRenderDeferedCommandBufferStep;
			l_textureLoadInfo.TextureKey = p_key;

			Texture* l_texture = Texture_alloc(&l_textureLoadInfo);
			TextureResourceWithCounter l_resouceWithCounter{};

			l_resouceWithCounter.Texture = l_texture;
			l_resouceWithCounter.UsageCounter.UsageCount = 0;
			p_textureResourceProvider->TextureResources.emplace(l_hash, l_resouceWithCounter);
		}

		TextureResourceWithCounter* l_resourceWithCounter = &p_textureResourceProvider->TextureResources.at(l_hash);
		_Utils::UsageCounter_use(&l_resourceWithCounter->UsageCounter);
		return l_resourceWithCounter->Texture;
	};


	void TextureResourceProvider_ReleaseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key)
	{
		size_t l_hash = TextureUniqueKey_buildHash(p_key);
		TextureResourceWithCounter* l_resourceWithCounter = &p_textureResourceProvider->TextureResources.at(l_hash);
		_Utils::UsageCounter_release(&l_resourceWithCounter->UsageCounter);
		if (l_resourceWithCounter->UsageCounter.UsageCount == 0)
		{
			Texture_free(&l_resourceWithCounter->Texture, p_textureResourceProvider->TextureResourceProviderDependencies.Device);
			p_textureResourceProvider->TextureResources.erase(l_hash);
		}
	};

	void TextureResourceProvider_Clear(TextureResourceProvider* p_textureResourceProvider)
	{
		if (p_textureResourceProvider->TextureResources.size() > 0)
		{
#ifndef NDEBUG
			std::string l_textureResourcesNotDisposed = "[";
			for (auto&& l_textureResourceEntry : p_textureResourceProvider->TextureResources)
			{
				l_textureResourcesNotDisposed += l_textureResourceEntry.second.Texture->TextureUniqueKey.TexturePath;
				l_textureResourcesNotDisposed += ",";
			}
			l_textureResourcesNotDisposed += "]";
			_Log::LogInstance->CoreLogger->warn("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.");
#endif

			for (auto&& l_textureResourceEntry : p_textureResourceProvider->TextureResources)
			{
				Texture_free(&l_textureResourceEntry.second.Texture, p_textureResourceProvider->TextureResourceProviderDependencies.Device);
			}
			p_textureResourceProvider->TextureResources.clear();
		}
	};
}