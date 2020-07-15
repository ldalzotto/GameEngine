#include "CameraSystem.h"

#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Segment/Segment.h"

#include "DataStructures/Specifications/VectorT.hpp"

extern "C"
{
#include "Functional/Hash/Hash.h"
}

#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "Render/RenderInterface.h"
#include "Render/RenderStep/PushCameraBuffer.h"
#include "Render/VulkanObjects/Hardware/Window/Window.h"

namespace _GameEngine::_ECS
{
	SystemV2Key CameraSystemKey = Core_Hash_string("CameraSystem");

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface);

	::_Core::SortedSequencerPriority CameraSystem_getUpdatePriority()
	{
		::_Core::VectorT<::_Core::SortedSequencerPriority> l_before;
		::_Core::VectorT_alloc(&l_before, 1);		
		::_Core::SortedSequencerPriority l_meshDrawBeforePriority = MeshDrawSystem_updatePriorityBefore();
		::_Core::VectorT_pushBack(&l_before, &l_meshDrawBeforePriority);
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void CameraSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->SystemKey = CameraSystemKey;

		p_systemV2AllocInfo->Update.Priority = CameraSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.OperationCallback = { cameraSystem_update, NULL } ;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&CameraType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformComponentType);
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface)
	{
		_ECS::SystemV2* l_cameraSystem = (_ECS::SystemV2*)p_cameraSystem;
		if (l_cameraSystem->EntityConfigurableContainer.FilteredEntities.size() > 0)
		{
			Entity* l_entity = *l_cameraSystem->EntityConfigurableContainer.FilteredEntities.at(0);
			TransformComponent* p_transform = ENTITY_GET_COMPONENT(TransformComponent, l_entity);
			Camera* p_camera = ENTITY_GET_COMPONENT(Camera, l_entity);

			{
				_Math::Vector3f l_worldPosition = Transform_getWorldPosition(&p_transform->Transform);

				_Math::Vector3f l_target;
				_Math::Vector3f l_foward = Transform_getForward(&p_transform->Transform);
				_Math::Vector3f_add(&l_worldPosition, &l_foward, &l_target);

				_Math::Vector3f l_up = Transform_getUp(&p_transform->Transform);
				_Math::Vector3f_mul(&l_up, -1.0f, &l_up);

				_Math::Matrix4x4f l_lookAt;
				_Math::Matrixf4x4_lookAt(&l_worldPosition, &l_target, &l_up, &l_lookAt);
				_Math::Matrixf4x4_inv(&l_lookAt, &p_camera->ViewMatrix);
			}


			p_camera->RenderInterface->PushCameraBuffer->CameraProjection.Projection = p_camera->ProjectionMatrix;
			p_camera->RenderInterface->PushCameraBuffer->CameraProjection.View = p_camera->ViewMatrix;
			_Render::PushCameraBuffer_pushToGPU(p_camera->RenderInterface->PushCameraBuffer, p_camera->RenderInterface->Device);
		}
	};

	Camera* CameraSystem_getCurrentActiveCamera(SystemV2* p_system)
	{
		if (p_system->EntityConfigurableContainer.FilteredEntities.size() > 0)
		{
			_ECS::Entity* l_entity = (*p_system->EntityConfigurableContainer.FilteredEntities.at(0));
			return ENTITY_GET_COMPONENT(Camera, l_entity);
		}
		return nullptr;
	};

	void CameraSystem_buildWorldSpaceRay(Camera* p_camera, _Math::Vector2f* p_screenPoint, _Math::Segment* out_ray)
	{
		_Math::Vector2f l_graphicsAPIPixelCoord;

		{
			_Math::Vector3f l_screenPoint3f = { p_screenPoint->x, p_screenPoint->y, 1.0f };
			_Math::Vector3f l_graphicsAPIPixelCoord3f;
			_Math::Matrix3x3f_mul(&p_camera->RenderInterface->Window->WindowToGraphicsAPIPixelCoordinates, &l_screenPoint3f, &l_graphicsAPIPixelCoord3f);
			l_graphicsAPIPixelCoord = { l_graphicsAPIPixelCoord3f.x, l_graphicsAPIPixelCoord3f.y };
		}

		_Math::Matrix4x4f l_screenToWorldMatrix;
		{
			_Math::Matrix4x4f l_tmp;
			_Math::Matrixf4x4_mul(&p_camera->ProjectionMatrix, &p_camera->ViewMatrix, &l_tmp);
			_Math::Matrixf4x4_inv(&l_tmp, &l_screenToWorldMatrix);
		}

		_Math::Vector4f l_beginScreenSpace = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, -1.0f, 1.0f };
		_Math::Vector4f l_beginWorldSpace;
		_Math::Matrixf4x4_mul(&l_screenToWorldMatrix, &l_beginScreenSpace, &l_beginWorldSpace);
		_Math::Vector4f_mul(&l_beginWorldSpace, 1.0f / l_beginWorldSpace.w, &l_beginWorldSpace);

		_Math::Vector4f l_endScreenSpace = { l_graphicsAPIPixelCoord.x, l_graphicsAPIPixelCoord.y, 1.0f, 1.0f };
		_Math::Vector4f l_endWorldSpace;
		_Math::Matrixf4x4_mul(&l_screenToWorldMatrix, &l_endScreenSpace, &l_endWorldSpace);
		_Math::Vector4f_mul(&l_endWorldSpace, 1.0f / l_endWorldSpace.w, &l_endWorldSpace);

		_Math::Vector3f_build(&l_beginWorldSpace, &out_ray->Begin);
		_Math::Vector3f_build(&l_endWorldSpace, &out_ray->End);
	};
}