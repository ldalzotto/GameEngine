#include "TransformRotateSystem.h"

#include "ECS_Impl/Components/Transform/Transform.h"
#include "ECS_Impl/Components/Transform/TransformRotate.h"
#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{
	void TransformRotateSystem_init(TransformRotateSystem* p_transformRotateSystem, ECS* p_ecs)
	{
		EntityConfigurableContainerInitInfo l_entityConfigurableContainerInitInfo{};
		l_entityConfigurableContainerInitInfo.ListenedComponentTypes = { TransformType, TransformRotateType };
		l_entityConfigurableContainerInitInfo.ECS = p_ecs;
		EntityConfigurableContainer_init(&p_transformRotateSystem->EntityConfigurableContainer, &l_entityConfigurableContainerInitInfo);
	};

	void TransformRotateSystem_free(TransformRotateSystem* p_transformRotateSystem)
	{
		EntityConfigurableContainer_free(&p_transformRotateSystem->EntityConfigurableContainer, p_transformRotateSystem->ECS);
	};


	void TransformRotationSystem_update(TransformRotateSystem* p_transformRotateSystem, float p_delta)
	{
		for (Entity* l_entity : p_transformRotateSystem->EntityConfigurableContainer.FilteredEntities)
		{
			Transform* l_transform = GET_COMPONENT(Transform, l_entity);
			TransformRotate* l_transformRotate = GET_COMPONENT(TransformRotate, l_entity);
			Transform_setLocalRotation(l_transform, l_transform->LocalRotation * glm::quat(l_transformRotate->Axis * l_transformRotate->Speed * p_delta));
		}

		/*
		#include "imgui.h"

		ImGui::Begin("My Second Tool");

		// Plot some values
		const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
		ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		for (int n = 0; n < 50; n++)
		{
			ImGui::Text("%04d: Some text", n);
		}
		ImGui::EndChild();
		ImGui::End();
		*/
	};
}