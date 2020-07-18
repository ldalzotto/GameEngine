#include "MeshRendererBoundGizmoSystem.h"

#include <stdlib.h>

#include "Algorithm/Compare/CompareAlgorithmT.hpp"

#include "GameEngineApplicationInterface.h"
#include "Render/RenderInterface.h"
#include "Render/Gizmo/Gizmo.h"
#include "EngineSequencers/EngineSequencers.h"

#include "ECS/ECS.h"
#include "ECS/EntityT.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRendererBound.h"

#include "Functional/Sequencer/SortedSequencer.hpp"

namespace _GameEngineEditor
{
	::_Core::SortedSequencerPriority meshRendererBoundGizmoSystem_priority()
	{
		return EDITOR_PRIORITY;
	}

	bool MeshRendererBoundGizmoSystemOperation_EntityEquals(MeshRendererBoundGizmoSystemOperation* p_left, _ECS::Entity** p_right, void* p_null)
	{
		return p_left->Entity == (*p_right);
	}


	void meshRendererBoundGizmoSystem_free(void* p_system, void* p_null)
	{
		MeshRendererBoundGizmoSystem* l_meshRendererBoundSystem = (MeshRendererBoundGizmoSystem*)p_system;
		_ECS::SystemHeader_free((_ECS::SystemHeader*)l_meshRendererBoundSystem);
		_ECS::EntityFilterT_free(&l_meshRendererBoundSystem->EntityFilter, l_meshRendererBoundSystem->SystemHeader.ECS);
		_Core::VectorT_free(&l_meshRendererBoundSystem->Operations);
		free(l_meshRendererBoundSystem);
	}

	void meshRendererBoundGizmoSystem_update(void* p_system, void* p_input)
	{
		MeshRendererBoundGizmoSystem* l_meshRendererBoundSystem = (MeshRendererBoundGizmoSystem*)p_system;
		GameEngineApplicationInterface* l_gameEngineApplicationInterface = (GameEngineApplicationInterface*)p_input;
		_Core::VectorIteratorT<MeshRendererBoundGizmoSystemOperation> l_operation = _Core::VectorT_buildIterator(&l_meshRendererBoundSystem->Operations);
		while (_Core::VectorIteratorT_moveNext(&l_operation))
		{
			_Math::Matrix4x4f l_localToWorld = _Math::Transform_getLocalToWorldMatrix(&l_operation.Current->TransformComponent->Transform);
			_Render::Gizmo_drawBox(l_gameEngineApplicationInterface->RenderInterface->Gizmo, &l_operation.Current->MeshRendererBound->BoundingBox, &l_localToWorld, false);
		}
	}

	void meshRendererBoundGizmoSystem_onEntityElligible(MeshRendererBoundGizmoSystem* p_system, _ECS::Entity* p_entity)
	{
		MeshRendererBoundGizmoSystemOperation l_operation{};
		l_operation.Entity = p_entity;
		l_operation.MeshRendererBound = *_ECS::EntityT_getComponent<_ECS::MeshRendererBound>(p_entity);
		l_operation.TransformComponent = *_ECS::EntityT_getComponent<_ECS::TransformComponent>(p_entity);
		_Core::VectorT_pushBack(&p_system->Operations, &l_operation);
	}

	void meshRendererBoundGizmoSystem_onEntityNotElligible(MeshRendererBoundGizmoSystem* p_system, _ECS::Entity* p_entity)
	{
		_Core::VectorT_eraseCompare(&p_system->Operations, _Core::ComparatorT<MeshRendererBoundGizmoSystemOperation, _ECS::Entity*, void>{ MeshRendererBoundGizmoSystemOperation_EntityEquals, &p_entity });
	}

	void MeshRendererBoundGizmoSystem_alloc(_GameEngine::UpdateSequencer* p_updateSequencer, _ECS::ECS* p_ecs)
	{
		MeshRendererBoundGizmoSystem* l_meshRendererBoundSystem = (MeshRendererBoundGizmoSystem*)malloc(sizeof(MeshRendererBoundGizmoSystem));
		l_meshRendererBoundSystem->SystemHeader.OnSystemDestroyed = { meshRendererBoundGizmoSystem_free , l_meshRendererBoundSystem };
		l_meshRendererBoundSystem->SystemHeader.Update = { meshRendererBoundGizmoSystem_priority(), {meshRendererBoundGizmoSystem_update, l_meshRendererBoundSystem} };
		_Core::VectorT_alloc(&l_meshRendererBoundSystem->Operations, 0);

		_ECS::EntityFilterT_alloc(&l_meshRendererBoundSystem->EntityFilter, p_ecs, l_meshRendererBoundSystem, meshRendererBoundGizmoSystem_onEntityElligible, meshRendererBoundGizmoSystem_onEntityNotElligible);
		_ECS::SystemHeader_init((_ECS::SystemHeader*)l_meshRendererBoundSystem, p_ecs, (_Core::SortedSequencer*)&p_updateSequencer->UpdateSequencer);
	};
}