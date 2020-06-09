#pragma once

#include "vulkan/vulkan.h"

#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/ShaderParameter.h"

#include "Render/Shader/DescriptorSetLayout.h"
#include "Render/VulkanObjects/Descriptor/DescriptorPool.h"
#include "Render/VulkanObjects/GraphcisPipeline/GraphicsPipeline.h"

using namespace _GameEngine;
using namespace _GameEngine::_Render;

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct Texture;
}

namespace _GameEngineEditor
{

	struct GizmoDrawStepParameters
	{
		Shader VertexShader;
		Shader FragmentShader;
		VertexInput VertexInput;
		UniformBufferParameter ModelMatrixParameter;

		Texture* OwnDepthTexture;

		DescriptorSetLayout DescriptorSetLayout;
		DescriptorPool DescriptorPool;
	};

	struct GizmoDrawStepDrawObject
	{
		VkPipelineLayout PipelineLayout;
		GraphicsPipeline GraphicsPipeline;
	};

	struct GizmoDrawStep
	{
		GizmoDrawStepParameters GizmoDrawStepParameters;
		GizmoDrawStepDrawObject GizmoDrawStepDrawObject;
	};

	void GizmoDrawStep_alloc(GizmoDrawStep* p_gizmoDrawStep, RenderInterface* p_renderInterface);
	void GizmoDrawStep_free(GizmoDrawStep* p_gizmoDrawStep, RenderInterface* p_renderInterface);

}