#pragma once

#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"
#include "Render/Mesh/Mesh.h"
#include "Render/GraphcisPipeline/GraphicsPipeline.h"

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_Render
{
	struct MeshUniformObject
	{
		glm::mat4x4 Model;
		glm::mat4x4 View;
		glm::mat4x4 Project;
	};

	struct MeshDrawCommand
	{
		Mesh* Mesh;
		GraphicsPipeline* UsedRenderPipeline;
		VulkanBuffer MeshUniformBuffer;
		VkDescriptorSet DescriptorSet;
	};

	struct MeshDrawStep
	{
		std::unordered_map<GraphicsPipeline*, std::vector<MeshDrawCommand*>> MeshDrawCommands;
	};

	void MeshDrawStep_addMeshDrawCommand(MeshDrawStep* p_meshDrawStep, MeshDrawCommand* p_meshDrawCommand);
	void MeshDrawStep_removeMeshDrawCommand(MeshDrawStep* p_meshDrawStep, MeshDrawCommand* p_meshDrawCommand);
	void MeshDrawStep_buildCommandBuffer(Render* p_render, MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}