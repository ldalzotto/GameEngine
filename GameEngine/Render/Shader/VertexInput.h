#pragma once


#include <vector>

#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct VertexInput
	{
		VkVertexInputBindingDescription VertexInputBinding;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
	};

	void VertexInput_buildInput(VertexInput* p_vertexInput);
	void VertexInput_buildGizmoInfo(VertexInput* p_vertexInput);
};