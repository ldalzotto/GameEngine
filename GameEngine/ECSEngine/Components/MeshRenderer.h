#pragma once

#include "MeshRenderer_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct MeshRendererInitInfo_TYP
{
	char* MeshResourcePath;
}MeshRendererInitInfo, * MeshRendererInitInfo_PTR;

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface*p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo);
void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface);