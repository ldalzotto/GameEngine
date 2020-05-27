#pragma once

#include "vulkan/vulkan.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/DescriptorSetLayout.h"

#include <string>

namespace _GameEngine::_Render
{
	struct Device;
	struct ShaderContainer;
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

	struct ShaderDependencies
	{
		ShaderContainer* ShaderContainer;
	};

	typedef std::string ShaderUniqueID;

	struct Shader
	{
		ShaderDependencies ShaderDependencies;
		ShaderUniqueID ShaderPath;
		ShaderType ShaderType;
		int UsageCounter;
	};

	struct ShaderAllocInfo
	{
		ShaderDependencies* ShaderDependencies;
		ShaderUniqueID ShaderPath;
		ShaderType ShaderType;
	};

	Shader* Shader_allocOrGet(ShaderAllocInfo* p_shaderInitInfo);
	void Shader_releaseOrFree(Shader** p_shader);

	VkShaderModule Shader_allocateShaderModule(Shader* p_shader, Device* p_device);
	void Shader_freeShaderModule(VkShaderModule p_shaderModule, Device* p_device);


	VkPipelineShaderStageCreateInfo Shader_buildShaderStageCreate(Shader* p_shader, VkShaderModule p_shaderModule);
}