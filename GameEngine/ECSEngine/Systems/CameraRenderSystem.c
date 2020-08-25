#include "CameraRenderSystem.h"
#include "EntityFilter.h"
#include "ECS.h"
#include "ECSEngine/Components/Types_def.h"
#include "RenderV2Interface.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/TransformC.h"
#include "ECSEngine/Components/TransformComponent.h"
#include "ECSEngine/Components/Camera.h"

void CameraRenderSystem_Alloc(CameraRenderSystem_PTR p_cameraRenderSystem, ECS* p_ecs)
{
	Arr_Alloc_CameraRenderSystemOperation(&p_cameraRenderSystem->Operations, 0);
	ECS_EntityFilter_Alloc_2c(&p_cameraRenderSystem->EntityFilter, CAMERA_TYPE, TRANSFORM_COMPONENT_TYPE);
	ECS_EntityFilter_Register(p_ecs, &p_cameraRenderSystem->EntityFilter);
};

void CameraRenderSystem_Free(CameraRenderSystem_PTR p_cameraRenderSystem, ECS* p_ecs)
{
	ECS_EntityFilter_UnRegister(p_ecs, &p_cameraRenderSystem->EntityFilter);
	ECS_EntityFilter_Free(&p_cameraRenderSystem->EntityFilter);
	Arr_Free_CameraRenderSystemOperation(&p_cameraRenderSystem->Operations);
};

void CameraRenderSystem_ConsumeECSEvents(CameraRenderSystem_PTR p_cameraRenderSystem)
{
	for (size_t i = 0; i < p_cameraRenderSystem->EntityFilter.EntityFilterEvents.Size; i++)
	{
		ECS_EntityFilterEvent_PTR l_event = &p_cameraRenderSystem->EntityFilter.EntityFilterEvents.Memory[i];
		switch (l_event->Type)
		{
		case EntityFilterEventType_ConditionsJustMet:
		{

			CameraRenderSystemOperation l_operation = {0};
			l_operation.Entity = l_event->Entity;
			// ECS_EventMessage_AddComponent_TYP

			ECS_GetComponent_Camera(l_operation.Entity, &l_operation.Camera);
			ECS_GetComponent_TransformComponent(l_operation.Entity, &l_operation.TransformComponent);
			Arr_PushBackRealloc_CameraRenderSystemOperation(&p_cameraRenderSystem->Operations, &l_operation);

			CAMERABUFFER_PTR l_cameraBuffer = l_operation.Camera->RenderInterface->GlobalBuffer.CameraBuffer;
			l_cameraBuffer->ViewMatrix = (Matrix4f_PTR)&l_operation.Camera->ViewMatrix;
			l_cameraBuffer->ProjectionMatrix = (Matrix4f_PTR)&l_operation.Camera->ProjectionMatrix;
			l_cameraBuffer->CameraFrustum = &l_operation.Camera->CameraFrustum;
		}
		break;

		case EntityFilterEventType_ConditionsJustNotMet:
		{
			for (size_t j = 0; j < p_cameraRenderSystem->Operations.Size; j++)
			{
				if (p_cameraRenderSystem->Operations.Memory[j].Entity == l_event->Entity)
				{
					Arr_Erase_CameraRenderSystemOperation(&p_cameraRenderSystem->Operations, j);
					break;
				}
			}
		}
		break;
		}
	}

	Arr_Clear((Array_PTR)&p_cameraRenderSystem->EntityFilter.EntityFilterEvents);
};

void CameraRenderSystem_Update(CameraRenderSystem_PTR p_system)
{
	CameraRenderSystem_ConsumeECSEvents(p_system);

	for (size_t i = 0; i < p_system->Operations.Size; i++)
	{
		CameraRenderSystemOperation_PTR l_operation = &p_system->Operations.Memory[i];

		TransformComponent* l_transform = l_operation->TransformComponent;
		Camera* l_camera = l_operation->Camera;
		CAMERABUFFER_PTR l_cameraBuffer = l_camera->RenderInterface->GlobalBuffer.CameraBuffer;

		{
			Vector3f tmp_vec3_0; Matrix4f tmp_mat4_0; Matrix3f tmp_mat3_0;

			Vector3f l_worldPosition; Transform_GetWorldPosition(&l_transform->Transform, &l_worldPosition);
			Vector3f l_target;
			Transform_GetForward(&l_transform->Transform, &l_target);
			Vec_Add_3f_3f(&l_worldPosition, &l_target, &l_target);
			Vector3f l_up;
			Transform_GetUp(&l_transform->Transform, &tmp_vec3_0);
			Vec_Mul_3f_1f(&tmp_vec3_0, -1.0f, &l_up);

			tmp_vec3_0 = (Vector3f) { 1.0f, 1.0f, 1.0f };
			Mat_LookAtRotation_F(&l_worldPosition, &l_target, &l_up, &tmp_mat3_0);
			Mat_TRS_Axis_M4F(&l_worldPosition, &tmp_mat3_0, &tmp_vec3_0, &tmp_mat4_0);
			Mat_Inv_M4F(&tmp_mat4_0, &l_camera->ViewMatrix);

			l_cameraBuffer->WorldPosition.Vec3 = l_worldPosition; l_cameraBuffer->WorldPosition.Vec3_w = 1.0f;
		}
	}
};

Camera* CameraSystem_getCurrentActiveCamera(CameraRenderSystem_PTR p_system)
{
	if (p_system->Operations.Size > 0)
	{
		return p_system->Operations.Memory[0].Camera;
	}
	return NULL;
};