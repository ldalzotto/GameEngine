
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "Render/Descriptor/DescriptorPool.h"
#include "Render/GraphcisPipeline/GraphicsPipeline.h"
#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/DescriptorSetLayout.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct SwapChain;
	struct CameraBufferSetupStep;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2_ExternalResources
	{
		Shader VertexShader;
		Shader FragmentShader;
	};

	struct DefaultMaterialV2
	{
		struct LocalInputParameters
		{
			VertexInput VertexInput;
			DescriptorSetLayout DescriptorSetLayout;
			DescriptorPool DescriptorPool;
		};

		struct FinalDrawObjects
		{
			VkPipelineLayout PipelineLayout;
			GraphicsPipeline GraphicsPipeline;
		};

		DefaultMaterialV2_ExternalResources ExternalResources;
		LocalInputParameters LocalInputParameters;
		FinalDrawObjects FinalDrawObjects;
	};

	struct DefaultMaterialV2AllocInfo
	{
		Device* Device;
		SwapChain* SwapChain;
		CameraBufferSetupStep* CameraBufferSetupStep;
	};

	void DefaultMaterial_alloc(DefaultMaterialV2* p_defaultMaterialV2, DefaultMaterialV2AllocInfo* p_defaultMaterialV2AllocInfo);
	void DefaultMaterial_free(DefaultMaterialV2* p_defaultMaterialV2, Device* p_device);
	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterialV2* p_defaultMaterialV2, DefaultMaterialV2AllocInfo* p_defaultMaterialV2AllocInfo);
};