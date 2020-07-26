#include "MeshRendererBoundSystem.h"

#include "Math/Box/BoxMath.h"

#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "EngineSequencers.h"

#include "Render/Mesh/Mesh.h"
#include "Render/Materials/MaterialInstance.h"

#include "Physics/PhysicsInterface.h"
#include "Physics/World/World.h"
#include "Physics/World/Collider/BoxCollider.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

#include "ECS_Impl/Systems/MeshDraw/MeshDrawSystem.h"

#include "ECS/System.h"

namespace _GameEngine::_ECS
{

	::_Core::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority()
	{

		_Core::SortedSequencerPriority l_beforePtr[1];
		_Core::ArrayT<_Core::SortedSequencerPriority> l_before = _Core::ArrayT_fromCStyleArray(l_beforePtr, 1);
		l_before.Size = 0;

		::_Core::ArrayT_pushBack(&l_before, MeshDrawSystem_updatePriorityBefore());
		return ::_Core::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	bool MeshRendererBoundOperation_EqualsEntity(MeshRendererBoundCalculationOperation* p_operation, Entity** p_entity, void* p_null)
	{
		return p_operation->Entity == *p_entity;
	}


	void meshRendererBoundSystem_onComponentAttached(void* p_system, Entity* p_entity)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		MeshRendererBoundCalculationOperation l_operation;
		{
			l_operation.Bound = EntityT_getComponent<MeshRendererBound>(p_entity);
			l_operation.MeshRenderer = EntityT_getComponent<MeshRenderer>(p_entity);
			l_operation.Entity = p_entity;
		}
		_Core::VectorT_pushBack(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate, &l_operation);

		l_operation.Bound->Boxcollider = new _Physics::BoxCollider();
		_Physics::BoxCollider l_boxCollider{};
		l_boxCollider.Box = &l_operation.Bound->BoundingBox;
		_ECS::TransformComponent* l_transformComponent = EntityT_getComponent<TransformComponent>(p_entity);
		l_boxCollider.Transform = &l_transformComponent->Transform;
		l_operation.Bound->Boxcollider = _Physics::BoxCollider_alloc(&l_boxCollider);
		_Physics::World_pushBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World, l_operation.Bound->Boxcollider);
	};

	void meshRendererBoundSystem_onComponentRemoved(void* p_system, Entity* p_entity)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		MeshRendererBound* l_rendererBound = EntityT_getComponent<MeshRendererBound>(p_entity);
		_Core::VectorT_eraseCompare(&l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate, _Core::ComparatorT<_ECS::MeshRendererBoundCalculationOperation, Entity*, void>{ MeshRendererBoundOperation_EqualsEntity, &p_entity });
		_Physics::World_removeBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World, l_rendererBound->Boxcollider);
	};

	void meshRendererBoundSystem_update(void* p_system, void* p_gameEngineInterface)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;

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

	void MeshrendererBoundSystem_free(void* p_system, void* p_null)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;

		EntityFilter_free(&l_meshrendererBoundSystem->EntityFilter, l_meshrendererBoundSystem->SystemHeader.ECS);
		_ECS::SystemHeader_free(&l_meshrendererBoundSystem->SystemHeader);
		free(l_meshrendererBoundSystem);
	};

	void MeshRendererBoundSystem_alloc(ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface, UpdateSequencer* p_updateSequencer)
	{
		MeshRendererBoundSystem* l_meshRendererBoundSystem = (MeshRendererBoundSystem*)calloc(1, sizeof(MeshRendererBoundSystem));
		l_meshRendererBoundSystem->SystemHeader.ECS = p_ecs;
		l_meshRendererBoundSystem->SystemHeader.Update = { MeshRendererBoundSystem_getUpdatePriority(), {meshRendererBoundSystem_update, l_meshRendererBoundSystem} };
		l_meshRendererBoundSystem->SystemHeader.OnSystemDestroyed = { MeshrendererBoundSystem_free, l_meshRendererBoundSystem };

		l_meshRendererBoundSystem->PhysicsInterface = p_physicsInterface;
		_Core::VectorT_alloc(&l_meshRendererBoundSystem->MeshRendererBoundsToCaluclate, 2);

		EntityFilter_alloc_2c(&l_meshRendererBoundSystem->EntityFilter, p_ecs, 
			&MeshRendererBoundType, &MeshRendererType, 
			l_meshRendererBoundSystem, meshRendererBoundSystem_onComponentAttached, meshRendererBoundSystem_onComponentRemoved);

		_ECS::SystemHeader_init(&l_meshRendererBoundSystem->SystemHeader, p_ecs, (_Core::SortedSequencer*)&p_updateSequencer->UpdateSequencer);
	};
}