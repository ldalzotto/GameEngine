
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "Math/Matrix/Matrix.h"

#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "VulkanObjects/GraphcisPipeline/GraphicsPipeline.h"
#include "Shader/Shader.h"
#include "Shader/VertexInput.h"
#include "Shader/DescriptorSetLayout.h"
#include "Shader/ShaderParameter.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct Material_ExternalResources
	{
		Shader VertexShader;
		Shader FragmentShader;
	};

	struct Material_InternalResources
	{
		Texture* DepthBufferTexture;
	};

	struct ModelProjection
	{
		_Math::Matrix4x4f Model;
	};

	enum MaterialType : uint8_t
	{
		MESH, GIZMO
	};

	struct Material_LocalInputParameters
	{
		VertexInput VertexInput;

		std::vector<ShaderParameter> ShaderParameters;

		DescriptorSetLayout DescriptorSetLayout;
		DescriptorPool DescriptorPool;
	};

	struct MaterialUniqueKey
	{
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
	};

	size_t MaterialUniqueKey_buildHash(MaterialUniqueKey* p_materialUniqueKey);

	struct Material
	{
		struct FinalDrawObjects
		{
			VkPipelineLayout PipelineLayout;
			GraphicsPipeline GraphicsPipeline;
		};

		MaterialUniqueKey MaterialUniqueKey;
		MaterialType MaterialType;
		Material_ExternalResources ExternalResources;
		Material_InternalResources InternalResources;
		Material_LocalInputParameters LocalInputParameters;
		FinalDrawObjects FinalDrawObjects;
	};

	struct MaterialAllocInfo
	{
		MaterialType MaterialType;
		std::string VertexShader;
		std::string FragmentShader;
		bool UseDepthBuffer;
		std::vector<ShaderParameter> ShaderParameters;
	};

	Material* Material_alloc(RenderInterface* p_renderInterface, MaterialAllocInfo* p_materialAllocInfo);
	void Material_free(Material** p_defaultMaterialV2, RenderInterface* p_renderInterface);
	void Material_reAllocGraphicsPipeline(Material* p_defaultMaterialV2, RenderInterface* p_renderInterface);
};