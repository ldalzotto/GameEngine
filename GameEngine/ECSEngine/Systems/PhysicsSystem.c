
#include "PhysicsSystem.h"

#include "Physics/PhysicsInterface.h"

#include "ECSEngine/Components/PhysicsBody.h"

#include "Physics/World/World.h"
#include "Physics/World/Collider/BoxCollider.h"
#include "EntityFilter.h"
#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/MeshRenderer.h"


void PhysicsSystem_Alloc(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs)
{
	Arr_Alloc_PhysicsSystemOperation(&p_physicsSystem->PhysicsOperations, 0);
	ECS_EntityFilter_Alloc_2c(&p_physicsSystem->PhysicsAwareFilter, PHYSICSBODY_COMPONENT_TYPE, MESHRENDERER_COMPONENT_TYPE);
	ECS_EntityFilter_Register(p_ecs, &p_physicsSystem->PhysicsAwareFilter);
};


void PhysicsSystem_ConsumeECSEvents(PhysicsSystem_PTR p_physicsSystem, PhysicsInterface_PTR p_physicsInterface)
{
	for (size_t i = 0; i < p_physicsSystem->PhysicsAwareFilter.EntityFilterEvents.Size; i++)
	{
		ECS_EntityFilterEvent_PTR l_event = &p_physicsSystem->PhysicsAwareFilter.EntityFilterEvents.Memory[i];

		switch (l_event->Type)
		{
		case EntityFilterEventType_ConditionsJustMet:
		{

			PhysicsBody_PTR l_physicsBody; ECS_GetComponent(l_event->Entity, PHYSICSBODY_COMPONENT_TYPE, (ECS_ComponentHeader_HANDLE*)&l_physicsBody);
			TransformComponent_PTR l_transformComponent; ECS_GetComponent(l_event->Entity, TRANSFORM_COMPONENT_TYPE, (ECS_ComponentHeader_HANDLE*)&l_transformComponent);
			MeshRenderer_PTR l_meshRenderer; ECS_GetComponent(l_event->Entity, MESHRENDERER_COMPONENT_TYPE, (ECS_ComponentHeader_HANDLE*)&l_meshRenderer);

			PhysicsSystemOperation l_operation = {0};
			l_operation.Entity = l_event->Entity;
			l_operation.BoxCollider = BoxCollider_alloc(&l_meshRenderer->MeshBoundingBox, (Transform_PTR)&l_transformComponent->Transform);
			l_physicsBody->Boxcollider = l_operation.BoxCollider;

			Arr_PushBackRealloc_PhysicsSystemOperation(&p_physicsSystem->PhysicsOperations, &l_operation);
			World_pushBoxCollider(p_physicsInterface->World, l_operation.BoxCollider);
		}
		break;
		case EntityFilterEventType_ConditionsJustNotMet:
		{
			for (size_t i = 0; i < p_physicsSystem->PhysicsOperations.Size; i++)
			{
				PhysicsSystemOperation_PTR l_physicsOperation = &p_physicsSystem->PhysicsOperations.Memory[i];
				if (l_physicsOperation->Entity == l_event->Entity)
				{
					World_removeBoxCollider(p_physicsInterface->World, l_physicsOperation->BoxCollider);
					BoxCollider_free(&l_physicsOperation->BoxCollider);
					Arr_Erase_PhysicsSystemOperation(&p_physicsSystem->PhysicsOperations, i);
					break;
				}
			}
		}
		break;
		}
	}

	Arr_Clear((Array_PTR)&p_physicsSystem->PhysicsAwareFilter.EntityFilterEvents);
};

void PhysicsSystem_Update(PhysicsSystem_PTR p_physicsSystem, PhysicsInterface_PTR p_physicsInterface)
{
	PhysicsSystem_ConsumeECSEvents(p_physicsSystem, p_physicsInterface);
};

void PhysicsSystem_Free(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs, PhysicsInterface_PTR p_physicsInterface)
{
	ECS_EntityFilter_UnRegister(p_ecs, &p_physicsSystem->PhysicsAwareFilter);

	PhysicsSystem_ConsumeECSEvents(p_physicsSystem, p_physicsInterface);

	ECS_EntityFilter_Free(&p_physicsSystem->PhysicsAwareFilter);

	Arr_Free((Array_PTR)&p_physicsSystem->PhysicsOperations);
};