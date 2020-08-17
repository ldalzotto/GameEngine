#include "TransformRotateSystem.h"

#include "GameEngineApplicationInterface.h"
#include "Clock/Clock.hpp"

#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Transform/TransformM.hpp"

#include "EngineSequencers.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

extern "C"
{
#include "v2/_interface/QuaternionC.h"
}


namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		_Core::SortedSequencerPriority l_beforePtr[2];
		_Core::ArrayT<_Core::SortedSequencerPriority> l_before = _Core::ArrayT_fromCStyleArray(l_beforePtr, 2);
		l_before.Size = 0;

		_Core::ArrayT_pushBack(&l_before, MeshDrawSystem_updatePriorityBefore());
		_Core::ArrayT_pushBack(&l_before, CameraSystem_getUpdatePriority());
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	bool TransformRotationSystemV2_EqualsEntity(TransformRotateOperation* p_operation, Entity** p_entity, void* p_null)
	{
		return p_operation->Entity == *p_entity;
	}


	void TransformRotationSystemV2_onComponentMatchAdded(void* p_transformRotateSystem, Entity* p_entity)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		TransformRotateOperation l_operation{};
		l_operation.Entity = p_entity;
		l_operation.TransformComponent = EntityT_getComponent<TransformComponent>(p_entity);
		l_operation.TransformRotate = EntityT_getComponent<TransformRotate>(p_entity);
		_Core::VectorT_pushBack(&l_transformRotateSystem->TransformRotateOperations, &l_operation);
	};

	void TransformRotationSystemV2_onComponentmathRemoved(void* p_transformRotateSystem, Entity* p_entity)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		_Core::VectorT_eraseCompare(&l_transformRotateSystem->TransformRotateOperations, _Core::ComparatorT<TransformRotateOperation, Entity*, void>{ TransformRotationSystemV2_EqualsEntity, &p_entity });
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface);

	void TransformRotateSystemV2_free(void* p_transformRotateSystem, void* p_null)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		SystemContainerV2_removeSystemV2(&l_transformRotateSystem->SystemHeader.ECS->SystemContainerV2, &l_transformRotateSystem->SystemHeader);
		EntityFilter_free(&l_transformRotateSystem->EntityFilter, l_transformRotateSystem->SystemHeader.ECS);
		free(l_transformRotateSystem);
	}

	void TransformRotateSystemV2_alloc(UpdateSequencer* p_updateSequencer, ECS* p_ecs)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)calloc(1, sizeof(TransformRotateSystem));
		l_transformRotateSystem->SystemHeader.ECS = p_ecs;
		l_transformRotateSystem->SystemHeader.OnSystemDestroyed = { TransformRotateSystemV2_free, l_transformRotateSystem };
		l_transformRotateSystem->SystemHeader.Update = { TransformRotateSystem_getUpdatePritoriy(), {TransformRotationSystemV2_update, l_transformRotateSystem} };

		_Core::VectorT_alloc(&l_transformRotateSystem->TransformRotateOperations, 0);

		EntityFilter_alloc_2c(&l_transformRotateSystem->EntityFilter, p_ecs,
			&TransformComponentType, &TransformRotateType,
			l_transformRotateSystem, TransformRotationSystemV2_onComponentMatchAdded, TransformRotationSystemV2_onComponentmathRemoved);

		_ECS::SystemHeader_init(&l_transformRotateSystem->SystemHeader, p_ecs, (_Core::SortedSequencer*)&p_updateSequencer->UpdateSequencer);
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		GameEngineApplicationInterface* l_gameEngineInterface = (GameEngineApplicationInterface*)p_gameEngineInterface;

		_Core::VectorIteratorT<TransformRotateOperation> l_operations = _Core::VectorT_buildIterator(&l_transformRotateSystem->TransformRotateOperations);
		while (_Core::VectorIteratorT_moveNext(&l_operations))
		{
			QUATERNION4F tmp_quat_0;
			QUATERNION4F l_newLocalRotation;
			Quat_RotateAround((VECTOR3F_PTR)&l_operations.Current->TransformRotate->Axis, l_operations.Current->TransformRotate->Speed * l_gameEngineInterface->Clock->DeltaTime, &tmp_quat_0);
			Quat_Mul(&l_operations.Current->TransformComponent->Transform.LocalRotation, &tmp_quat_0, &l_newLocalRotation);
			_MathV2::TransformM::setLocalRotation(&l_operations.Current->TransformComponent->Transform, &l_newLocalRotation);
		}
	};

}
