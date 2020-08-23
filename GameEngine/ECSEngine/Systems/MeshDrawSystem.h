#pragma once

#include "MeshDrawSystem_def.h"

#include "DataStructures/ARRAY.h"
#include "ECS_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

ARRAY_ALLOC_FUNCTION(MeshDrawSystemOperation, Array_MeshDrawSystemOperation_PTR, MeshDrawSystemOperation)
ARRAY_FREE_FUNCTION(MeshDrawSystemOperation, Array_MeshDrawSystemOperation_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(MeshDrawSystemOperation, Array_MeshDrawSystemOperation_PTR, MeshDrawSystemOperation)
ARRAY_ERASE_FUNCTION(MeshDrawSystemOperation, Array_MeshDrawSystemOperation_PTR, MeshDrawSystemOperation)

void MeshDrawSystem_Alloc(MeshDrawSystem_PTR p_meshDrawSystem, ECS* p_ecs);
void MeshDrawSystem_Free(MeshDrawSystem_PTR p_meshDrawSystem, ECS* p_ecs, RenderV2Interface* p_renderInterface);

void MeshDrawSystem_Update(MeshDrawSystem_PTR p_meshDrawSystem, RenderV2Interface* p_renderInterface);