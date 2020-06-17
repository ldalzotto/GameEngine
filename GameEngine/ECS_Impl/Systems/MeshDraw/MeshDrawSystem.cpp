
#include "MeshDrawSystem.h"

#include <cstdlib>
#include <ctime>

#include "Utils/Algorithm/Algorithm.h"

#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "EngineSequencers/EngineSequencers.h"

#include "RenderInterface.h"
#include "Materials/Material.h"
#include "Shader/ShaderParameterKeys.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceContainer.h"

#include "Render/Mesh/Mesh.h"
#include "Math/Box/Box.h"
#include "Math/Box/BoxMath.h"

namespace _GameEngine::_ECS
{
	_Utils::SortedSequencerPriority MeshDrawSystem_getUpdatePriority()
	{
		return UPDATE_PUSH_TO_RENDER_PRIORITY;
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_delta);

	void meshDrawSystem_onComponentsAttached(Entity* p_entity);
	void meshDrawSystem_onComponentsDetached(Entity* p_entity);

	void MeshDrawSystemV2_init(SystemV2AllocInfo* p_systemV2AllocInfo, ECS* p_ecs)
	{
		p_systemV2AllocInfo->ECS = p_ecs;
		p_systemV2AllocInfo->Update.Priority = MeshDrawSystem_getUpdatePriority();
		p_systemV2AllocInfo->Update.Callback = MeshDrawSystem_update;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ECS = p_ecs;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.alloc(2);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&MeshRendererType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.ListenedComponentTypes.push_back(&TransformType);
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesAdded = meshDrawSystem_onComponentsAttached;
		p_systemV2AllocInfo->EntityConfigurableContainerInitInfo.OnEntityThatMatchesComponentTypesRemoved = meshDrawSystem_onComponentsDetached;
	};

	void meshDrawSystem_onComponentsAttached(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_addMaterialInstance(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial, l_mesRenderer->MaterialInstance);
	};
	
	void meshDrawSystem_onComponentsDetached(Entity* p_entity)
	{
		MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, p_entity);
		_Render::MaterialInstanceContainer_removeMaterial(l_mesRenderer->RenderInterface->MaterialInstanceContainer, l_mesRenderer->MaterialInstance->SourceMaterial);
	};

	void MeshDrawSystem_update(void* p_meshDrawSystem, void* p_delta)
	{
		_ECS::SystemV2* l_meshDrawSystem = (_ECS::SystemV2*)p_meshDrawSystem;

		for (size_t i = 0; i < l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.size(); i++)
		{
			Entity** l_entity = l_meshDrawSystem->EntityConfigurableContainer.FilteredEntities.at(i);

			MeshRenderer* l_mesRenderer = GET_COMPONENT(MeshRenderer, *l_entity);
			Transform* l_transform = GET_COMPONENT(Transform, *l_entity);

			if (l_transform->HasChangedThisFrame)
			{
				_Render::ModelProjection l_meshUniform{};
				l_meshUniform.Model = _ECS::Transform_getLocalToWorldMatrix(l_transform);
				_Render::MaterialInstance_pushUniformBuffer(l_mesRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MODEL_BUFFER, &l_meshUniform);

				l_transform->HasChangedThisFrame = false;


				// BOX TEST
				_Render::Mesh* l_mesh = _Render::MaterialInstance_getMesh(l_mesRenderer->MaterialInstance, _Render::MATERIALINSTANCE_MESH_KEY);

				_Core::VectorT<_Math::Vector3f> l_vertices;
				l_vertices.alloc(l_mesh->Vertices.size());
				{
					for (size_t i = 0; i < l_vertices.capacity(); i++)
					{
						l_vertices.push_back(&l_mesh->Vertices.at(i).pos);
					}
					_Math::Box l_box{};
					_Math::Box_build(&l_box, &l_vertices);
				}
				l_vertices.free();
			}
		}
	};
}