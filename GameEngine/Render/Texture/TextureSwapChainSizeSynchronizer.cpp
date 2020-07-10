#include "TextureSwapChainSizeSynchronizer.h"

#include "RenderInterface.h"
extern "C"
{
#include "Log/Log.h"
}

namespace _GameEngine::_Render
{
	void TextureSwapChainSizeSynchronizer_addSynchronizer(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, Texture* p_texture, TextureAllocInfo* p_textureAllocInfo)
	{
		SwapChainSizeSynchronizer l_sync{};
		l_sync.InitalTextureAllocInfo = *p_textureAllocInfo;
		l_sync.Texture = p_texture;
		p_textureSwapChainSynchronizer->Syncronizers.push_back(l_sync);
	};
	
	void TextureSwapChainSizeSynchronizer_removeSynchronizer(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, Texture* p_texture)
	{
		for (size_t i = 0; i < p_textureSwapChainSynchronizer->Syncronizers.size(); i++)
		{
			SwapChainSizeSynchronizer* l_sync = &p_textureSwapChainSynchronizer->Syncronizers.at(i);
			if (l_sync->Texture == p_texture)
			{
				p_textureSwapChainSynchronizer->Syncronizers.erase(p_textureSwapChainSynchronizer->Syncronizers.begin() + i);
				break;
			}
		}
	};

	void TextureSwapChainSizeSynchronizer_onSwapChainSizeChanged(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer)
	{
		// We create a copy because texture alloc and realloc causes the initial synchronizer vector to change
		std::vector<SwapChainSizeSynchronizer> l_synchronizersCopy = std::vector<SwapChainSizeSynchronizer>(p_textureSwapChainSynchronizer->Syncronizers);

		for (size_t i = 0; i < l_synchronizersCopy.size(); i++)
		{
			SwapChainSizeSynchronizer* l_sync = &l_synchronizersCopy.at(i);
			Texture_free(l_sync->Texture, l_sync->InitalTextureAllocInfo.RenderInterface);
			Texture_alloc(&l_sync->Texture, &l_sync->InitalTextureAllocInfo);
		}
	};

	void TextureSwapChainSizeSynchronizer_free(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, RenderInterface* p_renderInterface)
	{
#ifndef NDEBUG
		if (p_textureSwapChainSynchronizer->Syncronizers.size() > 0)
		{
			MYLOG_PUSH(p_renderInterface->MyLog, LOGLEVEL_WARN, "Potential Memory Leak. The TextureSwapChainSizeSynchronizer wasn't empty when freed.");
		}
#endif
	};
}