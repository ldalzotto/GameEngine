#pragma once

#include <vector>

#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct SwapChainTexture
	{
		Texture Texture;
	};

	struct SwapChainTextureProvider
	{
		std::vector<SwapChainTexture*> SwapChainTextures;
	};

	void SwapChainTextureProvider_onSwapChainRebuilded(SwapChainTextureProvider* p_swapChainTextureProvider);

	SwapChainTexture* SwapChainTextureProvider_allocTexture(SwapChainTextureProvider* p_swapChainTextureProvider, TextureProceduralInstanceInfo* p_textureProceduralInstanceInfo);
	void SwapChainTextureProvider_freeTexture(SwapChainTextureProvider* p_swapChainTextureProvider, SwapChainTexture* p_swapChainTexture);
}