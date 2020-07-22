#include "TransformRotateSystem.h"

#include "GameEngineApplicationInterface.h"
#include "Clock/Clock.hpp"

#include "DataStructures/Specifications/VectorT.hpp"

#include "EngineSequencers.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

#include "Math/Quaternion/QuaternionMath.h"

namespace _GameEngine::_ECS
{
	::_Core::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		::_Core::VectorT<::_Core::SortedSequencerPriority> l_before;
		::_Core::VectorT_alloc(&l_before, 2);
		::_Core::VectorT_pushBack(&l_before, MeshDrawSystem_updatePriorityBefore());
		::_Core::VectorT_pushBack(&l_before, CameraSystem_getUpdatePriority());
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

		_Core::SortedSequencerT_addOperation(&p_updateSequencer->UpdateSequencer, (_Core::SortedSequencerOperationT<GameEngineApplicationInterface>*) & l_transformRotateSystem->SystemHeader.Update);
		SystemContainerV2_addSystemV2(&p_ecs->SystemContainerV2, &l_transformRotateSystem->SystemHeader);
	};

	void TransformRotationSystemV2_update(void* p_transformRotateSystem, void* p_gameEngineInterface)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		GameEngineApplicationInterface* l_gameEngineInterface = (GameEngineApplicationInterface*)p_gameEngineInterface;

		_Core::VectorIteratorT<TransformRotateOperation> l_operations = _Core::VectorT_buildIterator(&l_transformRotateSystem->TransformRotateOperations);
		while (_Core::VectorIteratorT_moveNext(&l_operations))
		{
			_Math::Quaternionf l_newLocalRotation;
			_Math::Quaternion_rotateAround(&l_operations.Current->TransformComponent->Transform.LocalRotation, 
					&l_operations.Current->TransformRotate->Axis, 
					l_operations.Current->TransformRotate->Speed * l_gameEngineInterface->Clock->DeltaTime, &l_newLocalRotation);

			_Math::Transform_setLocalRotation(&l_operations.Current->TransformComponent->Transform, l_newLocalRotation);
		}
	};

}