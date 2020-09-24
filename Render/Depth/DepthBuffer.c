#include "DepthBuffer.h"
#include "Objects/Texture/Texture.h"


void DepthBuffer_Realloc(DepthBuffer_PTR p_depthBuffer, uint32_t p_width, uint32_t p_height)
{
	Texture_Free_f(p_depthBuffer);
	Texture_Alloc_f(p_depthBuffer, p_width, p_height);
};

void DepthBuffer_Clear(DepthBuffer_PTR p_depthBuffer, CAMERABUFFER_PTR p_cameraBuffer)
{
	size_t l_size = (size_t)p_depthBuffer->Width * p_depthBuffer->Height;
	for (size_t i = 0; i < l_size; i++)
	{
		p_depthBuffer->Pixels.Memory[i] = *p_cameraBuffer->Far;
	}
};

char DepthBuffer_PushDepthValue(DepthBuffer_PTR p_depthBuffer, Vector2i_PTR p_pixel, float p_value)
{
	size_t l_pixelIndex = ((size_t)p_pixel->y * p_depthBuffer->Width) + p_pixel->x;
	if (p_depthBuffer->Pixels.Memory[l_pixelIndex] >= p_value)
	{
		p_depthBuffer->Pixels.Memory[l_pixelIndex] = p_value;
		return 1;
	}
	return 0;
};

void DepthBuffer_Free(DepthBuffer_PTR p_depthBuffer)
{
	Texture_Free_f(p_depthBuffer);
};