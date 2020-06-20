#pragma once

#include <vector>

#include "Texture.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct SwapChainSizeSynchronizer
	{
		Texture* Texture;
		TextureAllocInfo InitalTextureAllocInfo;
	};

	struct TextureSwapChainSizeSynchronizer
	{
		std::vector<SwapChainSizeSynchronizer> Syncronizers;
	};

	void TextureSwapChainSizeSynchronizer_addSynchronizer(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, Texture* p_texture, TextureAllocInfo* p_textureAllocInfo);
	void TextureSwapChainSizeSynchronizer_removeSynchronizer(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, Texture* p_texture);
	void TextureSwapChainSizeSynchronizer_onSwapChainSizeChanged(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer);
	void TextureSwapChainSizeSynchronizer_free(TextureSwapChainSizeSynchronizer* p_textureSwapChainSynchronizer, RenderInterface* p_renderInterface);
}
