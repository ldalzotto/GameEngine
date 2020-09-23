#pragma once

#include "Objects/RenderedObject_def.h"

typedef struct RENDEREDOBJECT_BUFFER_TYP
{
	Array_RenderedObjectHandle RenderedObjects;
}RENDEREDOBJECT_BUFFER, * RENDEREDOBJECT_BUFFER_PTR;

typedef struct RenderedObjectBuffers_TYP
{
	RENDEREDOBJECT_BUFFER NotShaded_NotTextured;
	RENDEREDOBJECT_BUFFER NotShaded_Textured;
	RENDEREDOBJECT_BUFFER FlatShaded_NotTextured;
	RENDEREDOBJECT_BUFFER FlatShaded_Textured;
}RenderedObjectBuffers, * RenderedObjectBuffers_PTR;