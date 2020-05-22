#pragma once

#include "vulkan/vulkan.h"
#include "Render/Device/Device.h"

#include <string>

namespace _GameEngine::_Render::_Shader
{
	struct ShaderDependencies
	{
		_Device::Device* Device;
	};

	enum class ShaderType
	{
		VERTEX = 0x01,
		FRAGMENT = 0x02
	};

	struct Shader
	{
		ShaderDependencies ShaderDependencies;
		VkShaderModule ShaderModule;
		ShaderType ShaderType;
	};

	Shader createShader(const ShaderDependencies& p_shaderDependencies, const ShaderType& p_shaderType, std::string p_compiledShaderFilePath);
	void freeShader(Shader* p_shader);
}