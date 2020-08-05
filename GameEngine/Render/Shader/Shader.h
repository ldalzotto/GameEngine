#pragma once

#include "Includes/VulkanIncludes.h"
#include "Shader/VertexInput.h"
#include "Shader/DescriptorSetLayout.h"

#include <string>

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	enum class ShaderType
	{
		VERTEX = 0x01,
		FRAGMENT = 0x02
	};

	struct ShaderInputDescription
	{
		VertexInput VertexInput;
		DescriptorSetLayout DescriptorSetLayout;
	};

	struct Shader
	{
		std::string ShaderPath;
		ShaderType ShaderType;
	};

	VkShaderModule Shader_allocateShaderModule(Shader* p_shader, Device* p_device);
	void Shader_freeShaderModule(VkShaderModule p_shaderModule, Device* p_device);

	VkPipelineShaderStageCreateInfo Shader_buildShaderStageCreate(Shader* p_shader, VkShaderModule p_shaderModule);
}