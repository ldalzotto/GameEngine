#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

namespace _GameEngine::_Render::_Shader
{
	struct VertexInput
	{
		VkVertexInputBindingDescription VertexInputBinding;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
	};

	void VertexInput_buildInput(VertexInput* p_vertexInput);
};