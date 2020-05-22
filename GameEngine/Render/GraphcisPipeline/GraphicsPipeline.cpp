#include "GraphicsPipeline.h"
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	void build(GraphicsPieline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies)
	{
		_Shader::ShaderDependencies l_shaderDependencies;
		l_shaderDependencies.Device = p_graphicsPipelineDependencies.Device;

		_Shader::Shader l_vertexShader = _Shader::createShader(l_shaderDependencies, _Shader::ShaderType::VERTEX, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv");
		_Shader::Shader l_fragmentShader = _Shader::createShader(l_shaderDependencies, _Shader::ShaderType::FRAGMENT, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv");

		VkPipelineShaderStageCreateInfo l_shaderStages[] = { buildShaderStageCreate(&l_vertexShader), buildShaderStageCreate(&l_fragmentShader) };

		_Shader::freeShader(&l_vertexShader);
		_Shader::freeShader(&l_fragmentShader);

	};

	VkPipelineShaderStageCreateInfo buildShaderStageCreate(_Shader::Shader* p_shader)
	{
		VkPipelineShaderStageCreateInfo l_pipelineShaderStageCreate{};
		l_pipelineShaderStageCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		if (p_shader->ShaderType == _Shader::ShaderType::VERTEX)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (p_shader->ShaderType == _Shader::ShaderType::FRAGMENT)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		l_pipelineShaderStageCreate.module = p_shader->ShaderModule;
		l_pipelineShaderStageCreate.pName = "main";

		return l_pipelineShaderStageCreate;
	};
}