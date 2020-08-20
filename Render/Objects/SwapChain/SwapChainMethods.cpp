#include "SwapChainMethods.hpp"

#include "RenderV2Interface.hpp"
extern "C"
{
#include "Objects/Texture/Texture.h"
}
#include "Objects/Window/Window.hpp"

namespace _RenderV2
{
	void SwapChainM::alloc(SwapChain* p_swapChain, RenderV2Interface* p_renderInterface)
	{
		p_swapChain->RenderInterface = p_renderInterface;
		_Core::ObserverT_alloc(&p_swapChain->OnSwapChainBuilded);
	};

	void SwapChainM::free(SwapChain* p_swapChain)
	{
		_Core::ObserverT_free(&p_swapChain->OnSwapChainBuilded);
		Texture_Free_3C(&p_swapChain->PresentTexture);
	};

	void SwapChainM::resize(SwapChain* p_swapChain, uint32_t p_width, uint32_t p_height)
	{
		if (p_swapChain->PresentTexture.Pixels.Memory)
		{
			Texture_Free_3C(&p_swapChain->PresentTexture);
		}
		Texture_Alloc_3C(&p_swapChain->PresentTexture, p_swapChain->RenderInterface->AppWindow->WindowSize.Width, p_swapChain->RenderInterface->AppWindow->WindowSize.Height);

		_Core::ObserverT_broadcast(&p_swapChain->OnSwapChainBuilded, p_swapChain->RenderInterface);
	};
}