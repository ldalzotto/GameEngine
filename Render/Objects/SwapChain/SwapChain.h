#pragma once

#include "SwapChain_def.h"

void SwapChain_Alloc(SwapChain* p_swapChain);
void SwapChain_Free(SwapChain* p_swapChain);
void SwapChain_RegisterOnSwapChainBuilded(SwapChain* p_swapChain, void(*p_callback)(void*, void*), void* p_closure);
void SwapChain_UnRegisterOnSwapChainBuilded(SwapChain* p_swapChain, void(*p_callback)(void*, void*), void* p_closure);
void SwapChain_Resize(SwapChain* p_swapChain, uint32_t p_width, uint32_t p_height);
