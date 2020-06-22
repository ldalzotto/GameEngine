#include "MeshRendererBoundSystem.h"

#include "Math/Box/BoxMath.h"

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

	_Utils::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority()
	{
		_Core::VectorT<_Utils::SortedSequencerPriority>  l_before;
		l_before.alloc(1);
		{
			auto l_index = MeshDrawSystem_updatePriorityBefore();
			l_before.push_back(&l_index);
		}
		return _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
	};

	void meshRendererBoundSystem_onComponentAttached(Entity* p_entity, void* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		MeshRendererBoundCalculationOperation l_operation;
		{
			l_operation.Bound = GET_COMPONENT(MeshRendererBound, p_entity);
			l_operation.MeshRenderer = GET_COMPONENT(MeshRenderer, p_entity);
		}
		l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.push_back(&l_operation);

		l_operation.Bound->Boxcollider = new _Physics::BoxCollider();
		_Physics::BoxCollider l_boxCollider{};
		l_boxCollider.Box = &l_operation.Bound->BoundingBox;
		_ECS::TransformComponent* l_transformComponent = GET_COMPONENT(TransformComponent, p_entity);
		l_boxCollider.Transform = &l_transformComponent->Transform;
		l_operation.Bound->Boxcollider = _Physics::BoxCollider_alloc(&l_boxCollider);
		_Physics::World_pushBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World , l_operation.Bound->Boxcollider);
	};

	void meshRendererBoundSystem_onComponentRemoved(Entity* p_entity, void* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system;
		_ECS::MeshRendererBound* l_meshRendererBound = GET_COMPONENT(MeshRendererBound, p_entity);
		_Physics::World_removeBoxCollider(l_meshrendererBoundSystem->PhysicsInterface->World, l_meshRendererBound->Boxcollider);
	};

	void meshRendererBoundSystem_onSystemDestroyed(SystemV2* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system->Child;
		l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.free();
	};

	void meshRendererBoundSystem_update(void* p_system, void* p_gameEngineInterface)
	{
		SystemV2* l_system = (SystemV2*)p_system;
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)((SystemV2*)p_system)->Child;

		for (size_t i = 0; i < l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.size(); i++)
		{
			MeshRendererBoundCalculationOperation* l_operation = l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.at(i);

			_Render::Mesh* l_mesh = _Render::MaterialInstance_getMesh(l_operation->MeshRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MESH_KEY);

			//TODO - This is not the most optimal as we copy vertices to a temporary vector.
			_Core::VectorT<_Math::Vector3f> l_vertices;
			l_vertices.alloc(l_mesh->Vertices.size());
			{
				for (size_t i = 0; i < l_vertices.capacity(); i++)
				{
					l_vertices.push_back(&l_mesh->Vertices.at(i).pos);
				}
				_Math::Box_build(&l_operation->Bound->BoundingBox, &l_vertices);


			}
			l_vertices.free();
		}

		l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.clear();
	};


	void MeshRendererBoundSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs, _Physics::PhysicsInterface* p_physicsInterface)
	{

		MeshRendererBoundSystem* l_meshRendererBoundSystem = (MeshRendererBoundSystem*)malloc(sizeof(MeshRendererBoundSystem));;
		l_meshRendererBoundSystem->PhysicsInterface = p_physicsInterface;
		l_meshRendererBoundSystem->MeshRendererBoundsToCaluclate.alloc(2);

		p_systemV2AllocInfo->ECS = p_ecs;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererBoundType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = meshRendererBoundSystem_onComponentAttached;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAddedUserdata = l_meshRendererBoundSystem;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = meshRendererBoundSystem_onComponentRemoved;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemovedUserData = l_meshRendererBoundSystem;

		p_systemV2AllocInfo->OnSystemDestroyed = meshRendererBoundSystem_onSystemDestroyed;
		p_systemV2AllocInfo->Child = l_meshRendererBoundSystem;
		p_systemV2AllocInfo->Update.Priority = MeshRendererBoundSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = meshRendererBoundSystem_update;
	};
}