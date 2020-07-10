#include "TextureResourceProvider.h"

extern "C"
{
#include "Include/CoreV2.h"
}
#include "MyLog/MyLog.h"
#include "RenderInterface.h"

namespace _GameEngine::_Render
{

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

			Texture* l_texture = Texture_alloc(&l_textureAllocInfo);
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
		size_t l_hash = TextureUniqueKey_Hash(p_key);
		TextureResourceWithCounter* l_resourceWithCounter = &p_textureResourceProvider->TextureResources.at(l_hash);
		_Utils::UsageCounter_release(&l_resourceWithCounter->UsageCounter);
		if (l_resourceWithCounter->UsageCounter.UsageCount == 0)
		{
			Texture_free(&l_resourceWithCounter->Texture, p_textureResourceProvider->RenderInterface);
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
			MYLOG_PUSH(p_textureResourceProvider->RenderInterface->MyLog, _Log::WARN, (char*)("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.").c_str());
#endif

			for (auto&& l_textureResourceEntry : p_textureResourceProvider->TextureResources)
			{
				Texture_free(&l_textureResourceEntry.second.Texture, p_textureResourceProvider->RenderInterface);
			}
			p_textureResourceProvider->TextureResources.clear();
		}
	};
}