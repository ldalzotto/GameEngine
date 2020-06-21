#include "MeshRendererBoundGizmoSystem.h"

#include <stdlib.h>
#include "Render/RenderInterface.h"
#include "Render/Gizmo/Gizmo.h"
#include "ECS/ECS.h"
#include "EngineSequencers/EngineSequencers.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

namespace _GameEngineEditor
{
	struct MeshRendererBoundGizmoSystem
	{
		_Render::Gizmo* Gizmo;
	};

	_Utils::SortedSequencerPriority meshRendererBoundGizmoSystem_priority()
	{
		return EDITR_PRIORITY;
	}

	void meshRendererBoundGizmoSystem_update(void* p_system, void* p_input)
	{
		_ECS::SystemV2* l_system = (_ECS::SystemV2*)p_system;
		MeshRendererBoundGizmoSystem* l_meshRendererBoundSystem = (MeshRendererBoundGizmoSystem*)l_system->Child;

		for (size_t i = 0; i < l_system->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			_ECS::Entity* l_entity = *l_system->EntityConfigurableContainer.FilteredEntities.at(i);
			_ECS::Transform* l_transform = (_ECS::Transform*)_ECS::Entity_getComponent(l_entity, _ECS::TransformType)->Child;
			_ECS::MeshRendererBound* l_meshRendererBound = (_ECS::MeshRendererBound*)_ECS::Entity_getComponent(l_entity, _ECS::MeshRendererBoundType)->Child;

			_Math::Matrix4x4f l_localToWorld = _ECS::Transform_getLocalToWorldMatrix(l_transform);
			_Render::Gizmo_drawBox(l_meshRendererBoundSystem->Gizmo, &l_meshRendererBound->BoundingBox, &l_localToWorld);
		}
	}

	void MeshRendererBoundGizmoSystem_init(_ECS::SystemV2AllocInfo* p_systemV2AllocInfo, _ECS::ECS* p_ecs, _Render::RenderInterface* p_renderInterface)
	{
		MeshRendererBoundGizmoSystem* l_meshRendererBoundSystem = (MeshRendererBoundGizmoSystem*)malloc(sizeof(MeshRendererBoundGizmoSystem));
		l_meshRendererBoundSystem->Gizmo = p_renderInterface->Gizmo;

		p_systemV2AllocInfo->ECS = p_ecs;

		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&_ECS::MeshRendererBoundType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&_ECS::MeshRendererType);

		p_systemV2AllocInfo->Child = l_meshRendererBoundSystem;
		p_systemV2AllocInfo->Update.Priority = meshRendererBoundGizmoSystem_priority();
		p_systemV2AllocInfo->Update.Callback = meshRendererBoundGizmoSystem_update;
	};
}