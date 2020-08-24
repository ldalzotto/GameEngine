#pragma once

#include "Log/Log_def.h"
#include "World/Collider/BoxCollider_def.h"

typedef struct World_TYP
{
	Log_PTR Log;
	Array_BoxColliderPTR BoxColliders;
}World, * World_PTR;
