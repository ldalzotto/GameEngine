#include "MeshDrawSystem.h"

#include "EntityFilter.h"
#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "RenderV2Interface.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "v2/_interface/TransformC.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/MeshRenderer.h"

void MeshDrawSystem_ConsumeECSEvents(MeshDrawSystem_PTR p_meshDrawSystem, RenderV2Interface_PTR p_renderInterface)
{
	for (size_t i = 0; i < p_meshDrawSystem->EntityFilter.EntityFilterEvents.Size; i++)
	{
		ECS_EntityFilterEvent_PTR l_event = &p_meshDrawSystem->EntityFilter.EntityFilterEvents.Memory[i];
		switch (l_event->Type)
		{
		case EntityFilterEventType_ConditionsJustMet:
		{
			MeshDrawSystemOperation l_meshDrawOperation;
			l_meshDrawOperation.Entity = l_event->Entity;
			
			ECS_GetComponent_TransformComponent(l_event->Entity, &l_meshDrawOperation.TransformComponent);
			ECS_GetComponent_MeshRenderer(l_event->Entity, &l_meshDrawOperation.MeshRenderer);
			l_meshDrawOperation.RenderedObject = (RenderedObject_PTR)malloc(sizeof(RenderedObject));
			l_meshDrawOperation.RenderedObject->Mesh = &l_meshDrawOperation.MeshRenderer->MeshResource->Mesh;
			l_meshDrawOperation.RenderedObject->MeshBoundingBox = &l_meshDrawOperation.MeshRenderer->MeshBoundingBox;
			l_meshDrawOperation.RenderedObject->Material = l_meshDrawOperation.MeshRenderer->Material;

			Arr_PushBackRealloc_MeshDrawSystemOperation(&p_meshDrawSystem->MeshDrawSystemOperations, &l_meshDrawOperation);
			Arr_PushBackRealloc_RenderedObjectHandle(&p_renderInterface->GlobalBuffer.RenderedObjectsBuffer->RenderedObjects, &l_meshDrawOperation.RenderedObject);
		}
		break;
		case EntityFilterEventType_ConditionsJustNotMet:
		{
			for (size_t j = 0; j < p_meshDrawSystem->MeshDrawSystemOperations.Size; j++)
			{
				MeshDrawSystemOperation_PTR l_meshDrawSystemOperation = &p_meshDrawSystem->MeshDrawSystemOperations.Memory[j];

				//TODO -> Adding a unique ID the the rendered object list
				// _Render::MaterialInstanceContainer_removeMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);

				if (l_meshDrawSystemOperation->Entity == l_event->Entity)
				{
					for (size_t k = 0; k < p_renderInterface->GlobalBuffer.RenderedObjectsBuffer->RenderedObjects.Size; k++)
					{
						RenderedObject_HANDLE l_renderedObject = p_renderInterface->GlobalBuffer.RenderedObjectsBuffer->RenderedObjects.Memory[k];
						if (l_renderedObject == l_meshDrawSystemOperation->RenderedObject)
						{
							free(l_renderedObject);
							Arr_Erase_RenderedObjectHandle(&p_renderInterface->GlobalBuffer.RenderedObjectsBuffer->RenderedObjects, k);
							break;
						}
					}

					Arr_Erase_MeshDrawSystemOperation(&p_meshDrawSystem->MeshDrawSystemOperations, j);
					break;
				}
			}
		}
		break;
		}
	}

	Arr_Clear(&p_meshDrawSystem->EntityFilter.EntityFilterEvents.array);
};


void MeshDrawSystem_Alloc(MeshDrawSystem_PTR p_meshDrawSystem, ECS* p_ecs)
{
	Arr_Alloc_MeshDrawSystemOperation(&p_meshDrawSystem->MeshDrawSystemOperations, 10);
	ECS_EntityFilter_Alloc_2c(&p_meshDrawSystem->EntityFilter, MESHRENDERER_COMPONENT_TYPE, TRANSFORM_COMPONENT_TYPE);
	ECS_EntityFilter_Register(p_ecs, &p_meshDrawSystem->EntityFilter);
};

void MeshDrawSystem_Free(MeshDrawSystem_PTR p_meshDrawSystem, ECS* p_ecs, RenderV2Interface* p_renderInterface)
{
	ECS_EntityFilter_UnRegister(p_ecs, &p_meshDrawSystem->EntityFilter);

	MeshDrawSystem_ConsumeECSEvents(p_meshDrawSystem, p_renderInterface);

	ECS_EntityFilter_Free(&p_meshDrawSystem->EntityFilter);
	Arr_Free_MeshDrawSystemOperation(&p_meshDrawSystem->MeshDrawSystemOperations);
};

void MeshDrawSystem_Update(MeshDrawSystem_PTR p_meshDrawSystem, RenderV2Interface* p_renderInterface)
{
	MeshDrawSystem_ConsumeECSEvents(p_meshDrawSystem, p_renderInterface);

	for (size_t i = 0; i < p_meshDrawSystem->MeshDrawSystemOperations.Size; i++)
	{
		MeshDrawSystemOperation* l_operation = &p_meshDrawSystem->MeshDrawSystemOperations.Memory[i];
		if (l_operation->TransformComponent->Transform.UserFlag_HasChanged)
		{
			Transform_GetLocalToWorldMatrix(&l_operation->TransformComponent->Transform, &l_operation->RenderedObject->ModelMatrix);
			l_operation->TransformComponent->Transform.UserFlag_HasChanged = false;
		}
	}

};