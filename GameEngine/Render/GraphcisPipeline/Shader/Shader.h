#pragma once


#include "vulkan/vulkan.h"
#include "Render/Hardware/Device/Device.h"


#include "Render/GraphcisPipeline/Shader/VertexInput.h"

#include <string>

namespace _GameEngine::_Render
{
	struct ShaderDependencies
	{
		Device* Device;
	};

	enum class ShaderType
	{
		VERTEX = 0x01,
		FRAGMENT = 0x02
	};

	struct VertexShaderDescription
	{
		VertexInput VertexInput;
	};

	struct Shader
	{
		ShaderDependencies ShaderDependencies;
		VkShaderModule ShaderModule;
		ShaderType ShaderType;
	};

	Shader Shader_create(const ShaderDependencies& p_shaderDependencies, const ShaderType& p_shaderType, std::string p_compiledShaderFilePath);
	void Shader_free(Shader* p_shader);

	VkPipelineShaderStageCreateInfo Shader_buildShaderStageCreate(Shader* p_shader);
}