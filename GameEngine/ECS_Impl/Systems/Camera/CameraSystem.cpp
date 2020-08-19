#include "CameraSystem.h"

#include "v2/Vector/VectorMath.hpp"

extern "C"
{
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/TransformC.h"
}

#include "DataStructures/Specifications/VectorT.hpp"

#include "Functional/Hash/Hash.hpp"

#include "EngineSequencers.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Camera/Camera.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "RenderV2Interface.hpp"
#include "Renderer/GlobalBuffers/CameraBuffer.hpp"

namespace _GameEngine::_ECS
{
	SystemV2Key CameraSystemKey = _Core::Hash_string("CameraSystem");

	::_Core::SortedSequencerPriority CameraSystem_getUpdatePriority()
	{
		_Core::SortedSequencerPriority l_beforePtr[1];
		_Core::ArrayT<_Core::SortedSequencerPriority> l_before = _Core::ArrayT_fromCStyleArray(l_beforePtr, 1);
		l_before.Size = 0;

		::_Core::SortedSequencerPriority l_meshDrawBeforePriority = MeshDrawSystem_updatePriorityBefore();
		::_Core::ArrayT_pushBack(&l_before, &l_meshDrawBeforePriority);
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	bool CameraSystemOperation_EqualsEntity(CameraSystemOperation* p_left, Entity** p_right, void* p_null) { return p_left->Entity == *p_right; }

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface);

	void cameraSystem_onEntityElligible(void* p_cameraSystem, Entity* p_entity)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		CameraSystemOperation l_operation{};
		l_operation.Entity = p_entity;
		l_operation.Camera = EntityT_getComponent<Camera>(p_entity);
		l_operation.TransformComponent = EntityT_getComponent<TransformComponent>(p_entity);
		_Core::VectorT_pushBack(&l_cameraSystem->Operations, &l_operation);

		_RenderV2::CameraBuffer* l_cameraBuffer = l_operation.Camera->RenderInterface->GlobalBuffer.CameraBuffer;
		l_cameraBuffer->ViewMatrix = (MATRIX4F_PTR)&l_operation.Camera->ViewMatrix;
		l_cameraBuffer->ProjectionMatrix = (MATRIX4F_PTR)&l_operation.Camera->ProjectionMatrix;
		l_cameraBuffer->CameraFrustum = &l_operation.Camera->CameraFrustum;
	}

	void cameraSystem_onEntityNoMoreElligible(void* p_cameraSystem, Entity* p_entity)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		_Core::VectorT_eraseCompare(&l_cameraSystem->Operations, _Core::ComparatorT<CameraSystemOperation, Entity*, void>{ CameraSystemOperation_EqualsEntity , &p_entity });
	}

	void CmaeraSystem_free(void* p_cameraSystem, void* p_null)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;
		EntityFilter_free(&l_cameraSystem->EntityFilter, l_cameraSystem->SystemHeader.ECS);
		_ECS::SystemHeader_free(&l_cameraSystem->SystemHeader);
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

		EntityFilter_alloc_2c(&l_cameraSystem->EntityFilter, p_ecs, &CameraType, &TransformComponentType, l_cameraSystem, cameraSystem_onEntityElligible, cameraSystem_onEntityNoMoreElligible);
		_ECS::SystemHeader_init(&l_cameraSystem->SystemHeader, p_ecs, (_Core::SortedSequencer*) & p_updateSequencer->UpdateSequencer);
	};

	void cameraSystem_update(void* p_cameraSystem, void* p_gameEngineInterface)
	{
		CameraSystem* l_cameraSystem = (CameraSystem*)p_cameraSystem;

		_Core::VectorIteratorT<CameraSystemOperation> l_operations = _Core::VectorT_buildIterator(&l_cameraSystem->Operations);
		while (_Core::VectorIteratorT_moveNext(&l_operations))
		{
			TransformComponent* l_transform = l_operations.Current->TransformComponent;
			Camera* l_camera = l_operations.Current->Camera;
			_RenderV2::CameraBuffer* l_cameraBuffer = l_camera->RenderInterface->GlobalBuffer.CameraBuffer;

			{
				_MathV2::Vector3<float> tmp_vec3_0; MATRIX4F tmp_mat4_0; MATRIX3F tmp_mat3_0;

				_MathV2::Vector3<float> l_worldPosition; Transform_GetWorldPosition(&l_transform->Transform, (VECTOR3F_PTR)&l_worldPosition);
				_MathV2::Vector3<float> l_target;
				Transform_GetForward(&l_transform->Transform, (VECTOR3F_PTR)&l_target);
				_MathV2::VectorM::add(&l_worldPosition, &l_target, &l_target);
				_MathV2::Vector3<float> l_up;
				Transform_GetUp(&l_transform->Transform, (VECTOR3F_PTR)&tmp_vec3_0);
				_MathV2::VectorM::mul(&tmp_vec3_0, -1.0f, &l_up);

				tmp_vec3_0 = { 1.0f, 1.0f, 1.0f };
				Mat_LookAtRotation_F((VECTOR3F_PTR)&l_worldPosition, (VECTOR3F_PTR)&l_target, (VECTOR3F_PTR)&l_up, &tmp_mat3_0);
				Mat_TRS_Axis_M4F((VECTOR3F_PTR)&l_worldPosition, &tmp_mat3_0, (VECTOR3F_PTR)&tmp_vec3_0, &tmp_mat4_0);
				Mat_Inv_M4F(&tmp_mat4_0, &l_camera->ViewMatrix);

				l_cameraBuffer->WorldPosition = _MathV2::VectorM::cast(&l_worldPosition, 1.0f);
			}
		}
	};

	Camera* CameraSystem_getCurrentActiveCamera(CameraSystem* p_system)
	{
		if (p_system->Operations.Size > 0)
		{
			_ECS::Entity* l_entity = (*_Core::VectorT_at(&p_system->Operations, 0)).Entity;
			return EntityT_getComponent<Camera>(l_entity);
		}
		return nullptr;
	};
}