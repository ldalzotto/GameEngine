#pragma once

#include "RenderedObject.h"
#include "Heap/RenderHeap.h"


void RendereableObject_PushToRenderEngine(RenderedObjectBuffers_PTR p_renderedObjectBuffers, RenderedObject_PTR p_renderableObject)
{
	switch (RRenderHeap.MaterialAllocator.array.Memory[p_renderableObject->Material.Handle].Type)
	{
	case MATERIAL_TYPE_NotShaded_NotTextured:
	{
		Arr_PushBackRealloc_RenderedObjectHandle(&p_renderedObjectBuffers->NotShaded_NotTextured.RenderedObjects, &p_renderableObject);
	}
	break;
	case MATERIAL_TYPE_NotShaded_Textured:
	{
		Arr_PushBackRealloc_RenderedObjectHandle(&p_renderedObjectBuffers->NotShaded_Textured.RenderedObjects, &p_renderableObject);
	}
	break;
	case MATERIAL_TYPE_FlatShaded_Textured:
	{
		Arr_PushBackRealloc_RenderedObjectHandle(&p_renderedObjectBuffers->FlatShaded_Textured.RenderedObjects, &p_renderableObject);
	}
	break;
	case MATERIAL_TYPE_FlatShaded_NotTextured:
	{
		Arr_PushBackRealloc_RenderedObjectHandle(&p_renderedObjectBuffers->FlatShaded_NotTextured.RenderedObjects, &p_renderableObject);
	}
	break;
	}
};

char RendereableObject_EraseFromRenderEngine(RenderedObjectBuffers_PTR p_renderedObjectBuffers, RenderedObject_PTR p_renderableObject)
{
	Array_RenderedObjectHandle_PTR l_involvedArray = NULL;
	switch (RRenderHeap.MaterialAllocator.array.Memory[p_renderableObject->Material.Handle].Type)
	{
	case MATERIAL_TYPE_NotShaded_NotTextured:
	{
		l_involvedArray = &p_renderedObjectBuffers->NotShaded_NotTextured.RenderedObjects;
	}
	break;
	case MATERIAL_TYPE_NotShaded_Textured:
	{
		l_involvedArray = &p_renderedObjectBuffers->NotShaded_Textured.RenderedObjects;
	}
	break;
	case MATERIAL_TYPE_FlatShaded_Textured:
	{
		l_involvedArray = &p_renderedObjectBuffers->FlatShaded_Textured.RenderedObjects;
	}
	break;
	case MATERIAL_TYPE_FlatShaded_NotTextured:
	{
		l_involvedArray = &p_renderedObjectBuffers->FlatShaded_NotTextured.RenderedObjects;
	}
	break;
	}

	if (l_involvedArray)
	{
		for (size_t i = 0; i < l_involvedArray->Size; i++)
		{
			RenderedObject_HANDLE l_renderedObject = l_involvedArray->Memory[i];
			if (l_renderedObject == p_renderableObject)
			{
				Arr_Erase_RenderedObjectHandle(l_involvedArray, i);
				return 1;
			}
		}
	}
	return 0;
};


//Arr_PushBackRealloc_RenderedObjectHandle(&p_renderInterface->GlobalBuffer.RenderedObjectsBuffer->RenderedObjects, &l_meshDrawOperation.RenderedObject);