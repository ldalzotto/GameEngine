#include "MeshRendererBoundSystem.h"

#include "DataStructures/VectorT.h"

#include "Math/Box/BoxMath.h"

#include "Render/RenderInterface.h"
#include "Render/Gizmo/Gizmo.h"
#include "Render/Mesh/Mesh.h"
#include "Render/Materials/MaterialInstance.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

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
		_Core::VectorT<MeshRendererBoundCalculationOperation> MeshRendererBoundsToCaluclate;
		_Render::Gizmo* Gizmo;
	};

	_Utils::SortedSequencerPriority MeshRendererBoundSystem_getUpdatePriority()
	{
		return 0;
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
	};

	void meshRendererBoundSystem_onSystemDestroyed(SystemV2* p_system)
	{
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)p_system->Child;
		l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.free();
	};

	void meshRendererBoundSystem_update(void* p_system, void* p_delta)
	{
		SystemV2* l_system = (SystemV2*)p_system;
		MeshRendererBoundSystem* l_meshrendererBoundSystem = (MeshRendererBoundSystem*)((SystemV2*)p_system)->Child;

		for (size_t i = 0; i < l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.size(); i++)
		{
			// BOX TEST
			MeshRendererBoundCalculationOperation* l_operation = l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.at(i);

			_Render::Mesh* l_mesh = _Render::MaterialInstance_getMesh(l_operation->MeshRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MESH_KEY);

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

		for (size_t i = 0; i < l_system->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			_ECS::Entity* l_entity = *l_system->EntityConfigurableContainer.FilteredEntities.at(i);
			_ECS::Transform* l_transform = (_ECS::Transform*)_ECS::Entity_getComponent(l_entity, _ECS::TransformType)->Child;
			_ECS::MeshRendererBound* l_meshRendererBound = (_ECS::MeshRendererBound*)_ECS::Entity_getComponent(l_entity, _ECS::MeshRendererBoundType)->Child;

			_Render::Gizmo_drawBox(l_meshrendererBoundSystem->Gizmo, &l_meshRendererBound->BoundingBox, &l_transform->LocalToWorldMatrix);

		}

		l_meshrendererBoundSystem->MeshRendererBoundsToCaluclate.clear();
	};


	void MeshRendererBoundSystem_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{

		MeshRendererBoundSystem* l_meshRendererBoundSystem = (MeshRendererBoundSystem*)malloc(sizeof(MeshRendererBoundSystem));;
		l_meshRendererBoundSystem->MeshRendererBoundsToCaluclate.alloc(2);
		l_meshRendererBoundSystem->Gizmo = p_renderInterface->Gizmo;

		p_systemV2AllocInfo->ECS = p_ecs;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererBoundType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = meshRendererBoundSystem_onComponentAttached;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAddedUserdata = l_meshRendererBoundSystem;

		p_systemV2AllocInfo->OnSystemDestroyed = meshRendererBoundSystem_onSystemDestroyed;
		p_systemV2AllocInfo->Child = l_meshRendererBoundSystem;
		p_systemV2AllocInfo->Update.Priority = MeshRendererBoundSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = meshRendererBoundSystem_update;
	};
}