#pragma once

#include "v2/_interface/BoxC_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "Component_def.h"
#include "ECS.h"
#include "Types_def.h"

typedef struct MeshRenderer_TYP
{
	ECS_ComponentHeader Header;
	MeshResource_HANDLE MeshResource;
	BoxF MeshBoundingBox;
}MeshRenderer, * MeshRenderer_PTR;

ECS_GETCOMPONENT_FUNCTION(MeshRenderer, MESHRENDERER_COMPONENT_TYPE, MeshRenderer_PTR);