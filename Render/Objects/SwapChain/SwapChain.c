#include "SwapChain.h"

#include "Objects/Texture/Texture.h"
#include "Objects/Texture/TextureSampling_inline.c"
#include "Objects/Window/Window.h"
#include "v2/_inline/ColorC_inline.c"
#include "v2/Math.h"
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

void SwapChain_PushTexture(SwapChain* p_swapChain, Texture3f_PTR p_pushedTexture)
{
	if ((p_pushedTexture->Height != p_swapChain->PresentTexture.Height) || (p_pushedTexture->Width != p_swapChain->PresentTexture.Width))
	{
		float l_xRatio = 1.0f / p_swapChain->PresentTexture.Width;
		float l_yRatio = 1.0f / p_swapChain->PresentTexture.Height;

		Vector2f l_samplingUV = {0};
		// float l_currentX = 0.0f; float l_currentY = 0.0f;

		for (int y = 0; y < p_swapChain->PresentTexture.Height; y++)
		{
			for (int x = 0; x < p_swapChain->PresentTexture.Width; x++)
			{
				Color3f_PTR l_sampledColor;
				_i_TextureSample_Point_3f_3f_upperLeft(p_pushedTexture, &l_samplingUV, &l_sampledColor);
				_i_Color_Convert_3F_3C(l_sampledColor, &p_swapChain->PresentTexture.Pixels.Memory[x + (y * p_swapChain->PresentTexture.Width)]);
				l_samplingUV.x += l_xRatio;
			}

			l_samplingUV.x = 0;
			l_samplingUV.y += l_yRatio;
		}
	}
	else
	{

		size_t l_size = (size_t)p_pushedTexture->Height * p_pushedTexture->Width;
		for (size_t i = 0; i < l_size; i++)
		{
			_i_Color_Convert_3F_3C(&p_pushedTexture->Pixels.Memory[i], &p_swapChain->PresentTexture.Pixels.Memory[i]);
		}
	}
};