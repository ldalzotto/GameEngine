#pragma once


#include <vector>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct VertexInput
	{
		VkVertexInputBindingDescription VertexInputBinding;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
	};

	void VertexInput_buildInput(VertexInput* p_vertexInput);
};