#pragma once

// #include "ECS/EntityFilter.hpp"

#include "Entity_def.h"
#include "EntityFilter_def.h"
#include "Objects/RenderedObject.h"
#include "ECSEngine/Components/TransformComponent_def.h"
#include "ECSEngine/Components/MeshRenderer_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct MeshDrawSystemOperation_TYP
{
	ECS_Entity_HANDLE Entity;
	TransformComponent_PTR TransformComponent;
	MeshRenderer_PTR MeshRenderer;
	RenderedObject_HANDLE RenderedObject;
}MeshDrawSystemOperation, * MeshDrawSystemOperation_PTR;

typedef struct Array_MeshDrawSystemOperation_TYP
{
	ARRAY_TYPE_DEFINITION(MeshDrawSystemOperation)
}Array_MeshDrawSystemOperation, * Array_MeshDrawSystemOperation_PTR;

typedef struct MeshDrawSystem_TYP
{
	ECS_EntityFilter EntityFilter;
	Array_MeshDrawSystemOperation MeshDrawSystemOperations;
}MeshDrawSystem, * MeshDrawSystem_PTR;
