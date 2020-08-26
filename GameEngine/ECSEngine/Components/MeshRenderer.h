#pragma once

#include "MeshRenderer_def.h"

#include "ECS.h"
#include "Types_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef struct MeshRendererInitInfo_TYP
{
	char* MeshResourcePath;
}MeshRendererInitInfo, * MeshRendererInitInfo_PTR;

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface*p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo);
void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface);

ECS_ALLOCCOMPONENT_FUNCTION(MeshRenderer, MESHRENDERER_COMPONENT_TYPE, MeshRenderer, MeshRenderer_PTR);
ECS_ADDCOMPONENT_FUNCTION(MeshRenderer, MeshRenderer_PTR);
ECS_GETCOMPONENT_FUNCTION(MeshRenderer, MESHRENDERER_COMPONENT_TYPE, MeshRenderer_PTR);