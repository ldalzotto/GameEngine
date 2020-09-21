#pragma once

#include "DepthBuffer_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"

void DepthBuffer_Realloc(DepthBuffer_PTR p_depthBuffer, uint32_t p_width, uint32_t p_height);
void DepthBuffer_Clear(DepthBuffer_PTR p_depthBuffer, CAMERABUFFER_PTR p_cameraBuffer);

char DepthBuffer_PushDepthValue(DepthBuffer_PTR p_depthBuffer, Vector2i_PTR p_pixel, float p_value);

void DepthBuffer_Free(DepthBuffer_PTR p_depthBuffer);