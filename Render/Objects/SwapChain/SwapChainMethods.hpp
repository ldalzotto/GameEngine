#pragma once

#include "SwapChain.hpp"

namespace _RenderV2
{
	struct RenderV2Interface;
}

namespace _RenderV2
{
	struct SwapChainM
	{
		static void alloc(SwapChain* p_swapChain, RenderV2Interface* p_renderInterface);
		static void free(SwapChain* p_swapChain);
		static void resize(SwapChain* p_swapChain, uint32_t p_width, uint32_t p_height);
	};
}
