#include "TextureResourceProvider.h"

#include "Log/Log.hpp"
#include "RenderInterface.h"

namespace _GameEngine::_Render
{
	void textureResourceProvider_disposeResource(TextureResourceProvider* p_textureResourceProvider, _Core::SharedRAIIT<Texture, TextureResourceProvider>* p_texture)
	{
		size_t l_hash = TextureUniqueKey_Hash(&p_texture->Resource->TextureUniqueKey);
		Texture_free(&p_texture->Resource, p_textureResourceProvider->RenderInterface);
		p_textureResourceProvider->TextureResources.erase(l_hash);
	};

	Texture* TextureResourceProvider_UseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key)
	{
		size_t l_hash = TextureUniqueKey_Hash(p_key);

		if (!p_textureResourceProvider->TextureResources.contains(l_hash))
		{
			TextureAllocInfo l_textureAllocInfo{};
			l_textureAllocInfo.TextureAllocationType = TextureAllocationType::FILE;
			l_textureAllocInfo.RenderInterface = p_textureResourceProvider->RenderInterface;
			l_textureAllocInfo.TextureKey.TexturePath = p_key->TexturePath;
			l_textureAllocInfo.TextureCreateInfo.TextureType = TextureType::COLOR;
			l_textureAllocInfo.TextureCreateInfo.TextureUsage = TextureUsage::SHADER_INPUT;

			_Core::SharedRAIIT<Texture, TextureResourceProvider> l_sharedTexture;
			l_sharedTexture.UsageCount = 0;
			l_sharedTexture.Resource = Texture_alloc(&l_textureAllocInfo);
			l_sharedTexture.ReleaseResource = { textureResourceProvider_disposeResource, p_textureResourceProvider };

			p_textureResourceProvider->TextureResources.emplace(l_hash, l_sharedTexture);
		}

		_Core::SharedRAIIT<Texture, TextureResourceProvider>* l_resourceWithCounter = &p_textureResourceProvider->TextureResources.at(l_hash);
		_Core::SharedRAIIT_use(l_resourceWithCounter);
		return l_resourceWithCounter->Resource;
	};


	void TextureResourceProvider_ReleaseResource(TextureResourceProvider* p_textureResourceProvider, TextureUniqueKey* p_key)
	{
		size_t l_hash = TextureUniqueKey_Hash(p_key);
		_Core::SharedRAIIT_release(&p_textureResourceProvider->TextureResources.at(l_hash));
	};

	void TextureResourceProvider_Clear(TextureResourceProvider* p_textureResourceProvider)
	{
		if (p_textureResourceProvider->TextureResources.size() > 0)
		{
#ifndef NDEBUG
			std::string l_textureResourcesNotDisposed = "[";
			for (auto&& l_textureResourceEntry : p_textureResourceProvider->TextureResources)
			{
				l_textureResourcesNotDisposed += l_textureResourceEntry.second.Resource->TextureUniqueKey.TexturePath;
				l_textureResourcesNotDisposed += ",";
			}
			l_textureResourcesNotDisposed += "]";
			MYLOG_PUSH(p_textureResourceProvider->RenderInterface->MyLog, _Core::LogLevel::LOG_WARN, (char*)("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.").c_str());
#endif

			for (auto&& l_textureResourceEntry : p_textureResourceProvider->TextureResources)
			{
				Texture_free(&l_textureResourceEntry.second.Resource, p_textureResourceProvider->RenderInterface);
			}
			p_textureResourceProvider->TextureResources.clear();
		}
	};
}