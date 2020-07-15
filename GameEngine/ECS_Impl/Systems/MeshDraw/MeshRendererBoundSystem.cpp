#include "MeshRendererBoundSystem.h"

#include "Math/Box/BoxMath.h"

#include "DataStructures/Specifications/VectorT.hpp"

#include "Render/Mesh/Mesh.h"
#include "Render/Materials/MaterialInstance.h"

#include "Physics/PhysicsInterface.h"
#include "Physics/World/World.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "ECS/System.h"

namespace _GameEngine::_ECS
{
	struct MeshRendererBoundCalculationOperation
	{
		MeshRenderer* MeshRenderer;
		MeshRendererBound* Bound;
	};

	struct MeshRendererBoundSystem
	{
		_Physics::PhysicsInterface* PhysicsInterface;
		_Core::VectorT<MeshRendererBoundCalculationOperation> MeshRendererBoundsToCaluclate;
	};

	::_Core::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority()
	{
		::_Core::VectorT<::_Core::SortedSequencerPriority> l_before;
		::_Core::VectorT_alloc(&l_before, 1);
		::_Core::VectorT_pushBack(&l_before, MeshDrawSystem_updatePriorityBefore());
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void meshRendererBoundSystem_onComponentAttached(Entity* p_entity, void* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		MeshRendererBoundCalculationOperation l_operation;
		{
			l_operation.Bound = ENTITY_GET_COMPONENT(MeshRendererBound, p_entity);
			l_operation.MeshRenderer = ENTITY_GET_COMPONENT(MeshRenderer, p_entity);
		}
		_Core::VectorT_pushBack(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate, &l_operation);

		l_operation.Bound->Boxcollider = new _Physics::BoxCollider();
		_Physics::BoxCollider l_boxCollider{};
		l_boxCollider.Box = &l_operation.Bound->BoundingBox;
		_ECS::TransformComponent* l_transformComponent = ENTITY_GET_COMPONENT(TransformComponent, p_entity);
		l_boxCollider.Transform = &l_transformComponent->Transform;
		l_operation.Bound->Boxcollider = _Physics::BoxCollider_alloc(&l_boxCollider);
		_Physics::World_pushBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World , l_operation.Bound->Boxcollider);
	};

	void meshRendererBoundSystem_onComponentRemoved(Entity* p_entity, void* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		_ECS::MeshRendererBound* l_meshRendererBound = ENTITY_GET_COMPONENT(MeshRendererBound, p_entity);
		_Physics::World_removeBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World, l_meshRendererBound->Boxcollider);
	};

	void meshRendererBoundSystem_onSystemDestroyed(SystemV2* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system->Child;
		_Core::VectorT_free(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate);
	};

	void meshRendererBoundSystem_update(void* p_system, void* p_gameEngineInterface)
	{
		SystemV2* l_system = (SystemV2*)p_system;
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)((SystemV2*)p_system)->Child;

		for (size_t i = 0; i < l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.Size; i++)
		{
			MeshRendererBoundCalculationOperation* l_operation = _Core::VectorT_at(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate, i);

			_Render::Mesh* l_mesh = _Render::MaterialInstance_getMesh(l_operation->MeshRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MESH_KEY);

			//TODO - This is not the most optimal as we copy vertices to a temporary vector.
			_Core::VectorT<_Math::Vector3f> l_vertices;
			_Core::VectorT_alloc(&l_vertices, l_mesh->Vertices.size());
			{
				for (size_t i = 0; i < l_vertices.Capacity; i++)
				{
					_Core::VectorT_pushBack(&l_vertices, &l_mesh->Vertices.at(i).pos);
				}
				_Math::Box_build(&l_operation->Bound->BoundingBox, &l_vertices);


			}
			_Core::VectorT_free(&l_vertices);
		}

		_Core::VectorT_clear(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate);
	};


	void MeshRendererBoundSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface)
	{

		MeshRendererBoundSystem* l_meshRendererBoundSystem = (MeshRendererBoundSystem*)malloc(sizeof(MeshRendererBoundSystem));;
		l_meshRendererBoundSystem->PhysicsInterface = p_physicsInterface;
		_Core::VectorT_alloc(&l_meshRendererBoundSystem->MeshRendererBoundsToCaluclate, 2);

		p_systemV2AllocInfo->ECS = p_ecs;

		{
			EntityConfigurableContainerInitInfo* l_entityContainerInfo = &p_systemV2AllocInfo->EntityConfigurableContainerInitInfo;

			l_entityContainerInfo->ECS = p_ecs;
			_Core::VectorT_alloc(&l_entityContainerInfo->ListenedComponentTypes, 2);
			_Core::VectorT_pushBack(&l_entityContainerInfo->ListenedComponentTypes, &MeshRendererBoundType);
			_Core::VectorT_pushBack(&l_entityContainerInfo->ListenedComponentTypes, &MeshRendererType);

			l_entityContainerInfo->OnEntityThatMatchesComponentTypesAdded = meshRendererBoundSystem_onComponentAttached;
			l_entityContainerInfo->OnEntityThatMatchesComponentTypesAddedUserdata = l_meshRendererBoundSystem;

			l_entityContainerInfo->OnEntityThatMatchesComponentTypesRemoved = meshRendererBoundSystem_onComponentRemoved;
			l_entityContainerInfo->OnEntityThatMatchesComponentTypesRemovedUserData = l_meshRendererBoundSystem;
		}

		p_systemV2AllocInfo->OnSystemDestroyed = meshRendererBoundSystem_onSystemDestroyed;
		p_systemV2AllocInfo->Child = l_meshRendererBoundSystem;
		p_systemV2AllocInfo->Update.Priority = MeshRendererBoundSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.OperationCallback = { meshRendererBoundSystem_update, NULL };
	};
}