#include "Shader.h"

#include <stdexcept>

#include "Log/Log.h"

#include "Utils/File/File.h"
using namespace _GameEngine::_Utils;

namespace _GameEngine::_Render::_Shader
{
	Shader createShader(const ShaderDependencies& p_shaderDependencies, const ShaderType& p_shaderType, std::string p_compiledShaderFilePath)
	{
		Shader l_shader{ };
		l_shader.ShaderDependencies = p_shaderDependencies;
		l_shader.ShaderType = p_shaderType;

		auto l_compiledShader = _File::readFile(p_compiledShaderFilePath);
		VkShaderModuleCreateInfo l_shaderModuleCreateInfo{};
		l_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		l_shaderModuleCreateInfo.codeSize = l_compiledShader.size();
		l_shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(l_compiledShader.data());

		if (vkCreateShaderModule(l_shader.ShaderDependencies.Device->LogicalDevice.LogicalDevice, &l_shaderModuleCreateInfo, nullptr, &l_shader.ShaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create shader module!"));
		}
		return l_shader;
	};

	void freeShader(Shader* p_shader)
	{
		vkDestroyShaderModule(p_shader->ShaderDependencies.Device->LogicalDevice.LogicalDevice, p_shader->ShaderModule, nullptr);
	};



	VkPipelineShaderStageCreateInfo buildShaderStageCreate(Shader* p_shader)
	{
		VkPipelineShaderStageCreateInfo l_pipelineShaderStageCreate{};
		l_pipelineShaderStageCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		if (p_shader->ShaderType == ShaderType::VERTEX)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (p_shader->ShaderType == ShaderType::FRAGMENT)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		l_pipelineShaderStageCreate.module = p_shader->ShaderModule;
		l_pipelineShaderStageCreate.pName = "main";

		return l_pipelineShaderStageCreate;
	};
}