#include "VertexInput.h"

#include "Mesh/Mesh.h"

namespace _GameEngine::_Render
{
	void VertexInput_buildInput(VertexInput* p_vertexInput)
	{
		p_vertexInput->VertexInputBinding = VkVertexInputBindingDescription{};
		p_vertexInput->VertexInputBinding.binding = 0;
		p_vertexInput->VertexInputBinding.stride = sizeof(Vertex);
		p_vertexInput->VertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		p_vertexInput->VertexInputAttributeDescriptions.resize(2);
		
		VkVertexInputAttributeDescription l_positionAttribute{};
		l_positionAttribute.binding = 0;
		l_positionAttribute.location = 0;
		l_positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
		l_positionAttribute.offset = offsetof(Vertex, pos);

		VkVertexInputAttributeDescription l_texCoordAttribute{};
		l_texCoordAttribute.binding = 0;
		l_texCoordAttribute.location = 1;
		l_texCoordAttribute.format = VK_FORMAT_R32G32_SFLOAT;
		l_texCoordAttribute.offset = offsetof(Vertex, TexCoord);

		p_vertexInput->VertexInputAttributeDescriptions.at(0) = l_positionAttribute;
		p_vertexInput->VertexInputAttributeDescriptions.at(1) = l_texCoordAttribute;
	};
};