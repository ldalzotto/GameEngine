#include "Shader.h"

#include <stdexcept>

#include "Render/Hardware/Device/Device.h"
#include "Render/Shader/ShaderContainer.h"

#include "Log/Log.h"
#include "Utils/File/File.h"

namespace _GameEngine::_Render
{
	Shader* Shader_allocOrGet(ShaderAllocInfo* p_shaderInitInfo)
	{
		if (!p_shaderInitInfo->ShaderDependencies->ShaderContainer->Shaders.contains(p_shaderInitInfo->ShaderPath))
		{
			Shader* l_shader = new Shader();
			l_shader->ShaderDependencies = *p_shaderInitInfo->ShaderDependencies;
			l_shader->ShaderType = p_shaderInitInfo->ShaderType;
			l_shader->ShaderPath = p_shaderInitInfo->ShaderPath;
			l_shader->UsageCounter = 1;
			l_shader->ShaderDependencies.ShaderContainer->Shaders[l_shader->ShaderPath] = l_shader;
			return l_shader;
		}
		else
		{
			return p_shaderInitInfo->ShaderDependencies->ShaderContainer->Shaders[p_shaderInitInfo->ShaderPath];
		}
	};

	void Shader_releaseOrFree(Shader** p_shader)
	{
		(*p_shader)->UsageCounter -= 1;
		if ((*p_shader)->UsageCounter <= 0)
		{
			(*p_shader)->ShaderDependencies.ShaderContainer->Shaders.erase((*p_shader)->ShaderPath);
			delete (*p_shader);
			(*p_shader) = nullptr;
		}
	};

	VkPipelineShaderStageCreateInfo Shader_buildShaderStageCreate(Shader* p_shader, VkShaderModule p_shaderModule)
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

		l_pipelineShaderStageCreate.module = p_shaderModule;
		l_pipelineShaderStageCreate.pName = "main";

		return l_pipelineShaderStageCreate;
	};

	VkShaderModule Shader_allocateShaderModule(Shader* p_shader, Device* p_device)
	{
		VkShaderModule l_shaderModule;
		auto l_compiledShader = _Utils::File_readFile(p_shader->ShaderPath);
		VkShaderModuleCreateInfo l_shaderModuleCreateInfo{};
		l_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		l_shaderModuleCreateInfo.codeSize = l_compiledShader.size();
		l_shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(l_compiledShader.data());

		if (vkCreateShaderModule(p_device->LogicalDevice.LogicalDevice, &l_shaderModuleCreateInfo, nullptr, &l_shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create shader module!"));
		}
		return l_shaderModule;
	};

	void Shader_freeShaderModule(VkShaderModule p_shaderModule, Device* p_device)
	{
		vkDestroyShaderModule(p_device->LogicalDevice.LogicalDevice, p_shaderModule, nullptr);
	};
}