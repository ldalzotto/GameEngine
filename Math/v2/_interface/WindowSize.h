#pragma once

#include "WindowSize_def.h"

void WindowSize_Precalculate_uint16t(uint16_t p_width, uint16_t p_height, WindowSize_PTR out_windowSize);
void WindowSize_GraphicsAPIToPixel(const WindowSize* p_windowSize, float p_x, float p_y, int* out_x, int* out_y);
void WindowSize_PixelToGraphicsAPI(const WindowSize* p_windowSize, int p_x, int p_y, float* out_x, float* out_y);