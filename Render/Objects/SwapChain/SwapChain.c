#include "SwapChain.h"

#include "Objects/Texture/Texture.h"
#include "Objects/Window/Window.h"
#include "Functional/Callback/Observer.h"

void SwapChain_Alloc(SwapChain* p_swapChain)
{
	Observer_Alloc(&p_swapChain->OnSwapChainBuilded);
};

void SwapChain_Free(SwapChain* p_swapChain)
{
	Observer_Free(&p_swapChain->OnSwapChainBuilded);
	Texture_Free_3C(&p_swapChain->PresentTexture);
};

void SwapChain_RegisterOnSwapChainBuilded(SwapChain* p_swapChain, void(*p_callback)(void*, void*), void* p_closure)
{
	Callback l_c = { (void (*)(void*, void*))p_callback, p_closure };
	Observer_Register(&p_swapChain->OnSwapChainBuilded, &l_c);
};

void SwapChain_UnRegisterOnSwapChainBuilded(SwapChain* p_swapChain, void(*p_callback)(void*, void*), void* p_closure)
{
	Callback l_c = { (void (*)(void*, void*))p_callback, p_closure };
	Observer_UnRegister(&p_swapChain->OnSwapChainBuilded, &l_c);
};

void SwapChain_Resize(SwapChain* p_swapChain, uint32_t p_width, uint32_t p_height)
{
	if (p_swapChain->PresentTexture.Pixels.Memory)
	{
		Texture_Free_3C(&p_swapChain->PresentTexture);
	}
	Texture_Alloc_3C(&p_swapChain->PresentTexture, p_width, p_height);

	Observer_Broadcast(&p_swapChain->OnSwapChainBuilded, NULL);
};