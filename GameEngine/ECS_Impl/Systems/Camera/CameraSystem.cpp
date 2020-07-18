#include "CameraSystem.h"

#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Segment/Segment.h"

#include "DataStructures/Specifications/VectorT.hpp"

#include "Functional/Hash/Hash.hpp"

#include "EngineSequencers.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "Render/RenderInterface.h"
#include "Render/RenderStep/PushCameraBuffer.h"
#include "Render/VulkanObjects/Hardware/Window/Window.h"

namespace _GameEngine::_ECS
{
	SystemV2Key CameraSystemKey = _Core::Hash_string("CameraSystem");

	::_Core::SortedSequencerPriority CameraSystem_getUpdatePriority()
	{
		::_Core::VectorT<::_Core::SortedSequencerPriority> l_before;
		::_Core::VectorT_alloc(&l_before, 1);		
		::_Core::SortedSequencerPriority l_meshDrawBeforePriority = MeshDrawSystem_updatePriorityBefore();
		::_Core::VectorT_pushBack(&l_before, &l_meshDrawBeforePriority);
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	bool CameraSystemOperation_EqualsEntity(CameraSystemOperation* p_left, Entity** p_right, void* p_null) { return p_left->Entity == *p_right; }

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface);

	void cameraSystem_onEntityElligible(void* p_cameraSystem, Entity* p_entity)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		CameraSystemOperation l_operation{};
		l_operation.Entity = p_entity;
		l_operation.Camera = *EntityT_getComponent<Camera>(p_entity);
		l_operation.TransformComponent = *EntityT_getComponent<TransformComponent>(p_entity);
		_Core::VectorT_pushBack(&l_cameraSystem->Operations, &l_operation);
	}

	void cameraSystem_onEntityNoMoreElligible(void* p_cameraSystem, Entity* p_entity)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		_Core::VectorT_eraseCompare(&l_cameraSystem->Operations, _Core::ComparatorT<CameraSystemOperation, Entity*, void>{ CameraSystemOperation_EqualsEntity , &p_entity});
	}

	void CmaeraSystem_free(void* p_cameraSystem, void* p_null)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		EntityFilter_free(&l_cameraSystem->EntityFilter, l_cameraSystem->SystemHeader.ECS);
		SystemContainerV2_removeSystemV2(&l_cameraSystem->SystemHeader.ECS->SystemContainerV2, &l_cameraSystem->SystemHeader);
		_Core::VectorT_free(&l_cameraSystem->Operations);
		free(l_cameraSystem);
	}

	void CameraSystem_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)malloc(sizeof(CameraSystem));
		_Core::VectorT_alloc(&l_cameraSystem->Operations, 0);

		l_cameraSystem->SystemHeader.ECS = p_ecs;
		l_cameraSystem->SystemHeader.OnSystemDestroyed = { CmaeraSystem_free, l_cameraSystem };
		l_cameraSystem->SystemHeader.SystemKey = CameraSystemKey;
		l_cameraSystem->SystemHeader.Update = { CameraSystem_getUpdatePriority() , {cameraSystem_update, l_cameraSystem} };

		_Core::VectorT_alloc(&l_cameraSystem->EntityFilter.ListenedComponentTypes, 2);
		_Core::VectorT_pushBack(&l_cameraSystem->EntityFilter.ListenedComponentTypes, &CameraType);
		_Core::VectorT_pushBack(&l_cameraSystem->EntityFilter.ListenedComponentTypes, &TransformComponentType);
		l_cameraSystem->EntityFilter.OnEntityThatMatchesComponentTypesAdded = { cameraSystem_onEntityElligible , l_cameraSystem };
		l_cameraSystem->EntityFilter.OnEntityThatMatchesComponentTypesRemoved = { cameraSystem_onEntityNoMoreElligible , l_cameraSystem };
		EntityFilter_init(&l_cameraSystem->EntityFilter, p_ecs);

		SystemContainerV2_addSystemV2(&p_ecs->SystemContainerV2, &l_cameraSystem->SystemHeader);
		_Core::SortedSequencerT_addOperation(&p_updateSequencer->UpdateSequencer, (_Core::SortedSequencerOperationT<GameEngineApplicationInterface>*) &l_cameraSystem->SystemHeader.Update);
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		_Core::VectorIteratorT<CameraSystemOperation> l_operations = _Core::VectorT_buildIterator(&l_cameraSystem->Operations);
		while (_Core::VectorIteratorT_moveNext(&l_operations))
		{
			TransformComponent* l_transform = l_operations.Current->TransformComponent;
			Camera* l_camera = l_operations.Current->Camera;

			{
				_Math::Vector3f l_worldPosition = Transform_getWorldPosition(&l_transform->Transform);

				_Math::Vector3f l_target;
				_Math::Vector3f l_foward = Transform_getForward(&l_transform->Transform);
				_Math::Vector3f_add(&l_worldPosition, &l_foward, &l_target);

				_Math::Vector3f l_up = Transform_getUp(&l_transform->Transform);
				_Math::Vector3f_mul(&l_up, -1.0f, &l_up);

				_Math::Matrix4x4f l_lookAt;
				_Math::Matrixf4x4_lookAt(&l_worldPosition, &l_target, &l_up, &l_lookAt);
				_Math::Matrixf4x4_inv(&l_lookAt, &l_camera->ViewMatrix);
			}

			l_camera->RenderInterface->PushCameraBuffer->CameraProjection.Projection = l_camera->ProjectionMatrix;
			l_camera->RenderInterface->PushCameraBuffer->CameraProjection.View = l_camera->ViewMatrix;
			_Render::PushCameraBuffer_pushToGPU(l_camera->RenderInterface->PushCameraBuffer, l_camera->RenderInterface->Device);
		}
	};

	Camera* CameraSystem_getCurrentActiveCamera(CameraSystem* p_system)
	{
		if (p_system->Operations.Size > 0)
		{
			_ECS::Entity* l_entity = (*_Core::VectorT_at(&p_system->Operations, 0)).Entity;
			return *EntityT_getComponent<Camera>(l_entity);
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