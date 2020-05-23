#pragma once

#include <vector>

#include "vulkan/vulkan.hpp"
#include "glm/glm.hpp"

namespace _GameEngine::_Render::_Shader
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct VertexInput
	{
		VkVertexInputBindingDescription VertexInputBinding;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
	};

	void VertexInput_buildInput(VertexInput* p_vertexInput);
};