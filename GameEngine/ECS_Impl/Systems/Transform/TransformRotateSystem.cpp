#include "TransformRotateSystem.h"

#include "EngineSequencers/EngineSequencers.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"
#include "ECS_Impl/Systems/Camera/CameraSystem.h"

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority TransformRotateSystem_getUpdatePritoriy()
	{
		std::vector<_Utils::SortedSequencerPriority> l_before{
			MeshDrawSystem_getUpdatePriority(),
			CameraSystem_getUpdatePriority()
		};
		return _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void TransformRotationSystem_update(void* p_transformRotateSystem, void* p_delta);

	void TransformRotateSystem_init(TransformRotateSystem* p_transformRotateSystem, ECS* p_ecs)
	{
		p_transformRotateSystem->ECS = p_ecs;

		EntityConfigurableContainerInitInfo l_entityConfigurableContainerInitInfo{};
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformType);
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformRotateType);
		l_entityConfigurableContainerInitInfo.ECS = p_ecs;

		p_transformRotateSystem->Update.Priority = TransformRotateSystem_getUpdatePritoriy();
		p_transformRotateSystem->Update.Callback = TransformRotationSystem_update;
		p_transformRotateSystem->Update.Closure = p_transformRotateSystem;

		_Utils::SortedSequencer_addOperation(&p_ecs->UpdateSequencer->UpdateSequencer, &p_transformRotateSystem->Update);

		EntityConfigurableContainer_init(&p_transformRotateSystem->EntityConfigurableContainer, &l_entityConfigurableContainerInitInfo);
	};

	void TransformRotateSystem_free(System* p_system)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_system->_child;
		EntityConfigurableContainer_free(&l_transformRotateSystem->EntityConfigurableContainer, l_transformRotateSystem->ECS);
		delete l_transformRotateSystem;
		p_system->_child = nullptr;
	};


	void TransformRotationSystem_update(void* p_transformRotateSystem, void* p_delta)
	{
		TransformRotateSystem* l_transformRotateSystem = (TransformRotateSystem*)p_transformRotateSystem;
		float l_delta = *(float*)p_delta;

		for (size_t i = 0; i < l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_transformRotateSystem->EntityConfigurableContainer.FilteredEntities.at(i);
			Transform* l_transform = GET_COMPONENT(Transform, *l_entity);
			TransformRotate* l_transformRotate = GET_COMPONENT(TransformRotate, *l_entity);
			Transform_setLocalRotation(l_transform, l_transform->LocalRotation * glm::quat(l_transformRotate->Axis * l_transformRotate->Speed * l_delta));
		}
	};

	System* TransformRotateSystem_alloc(ECS* p_ecs)
	{
		SystemAllocInfo l_systemAllocInfo{};
		l_systemAllocInfo.Child = new TransformRotateSystem();
		l_systemAllocInfo.OnSystemFree = TransformRotateSystem_free;
		System* l_system = SystemContainer_allocSystem(&p_ecs->SystemContainer, &l_systemAllocInfo);
		TransformRotateSystem* l_transformRotationSystem = (TransformRotateSystem*)l_system->_child;
		TransformRotateSystem_init(l_transformRotationSystem, p_ecs);
		return l_system;
	};
}