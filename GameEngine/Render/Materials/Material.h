
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "v2/Matrix/Matrix.hpp"

#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "VulkanObjects/GraphcisPipeline/GraphicsPipeline.h"
#include "Shader/Shader.h"
#include "Shader/VertexInput.h"
#include "Shader/DescriptorSetLayout.h"
#include "Shader/ShaderParameter.h"
#include "Materials/MaterialDrawFunctions.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct Material_GlobalResources
	{
		Texture* DepthBufferTexture;
	};

	struct Material_ExternalResources
	{
		Shader VertexShader;
		Shader FragmentShader;
	};

	struct ModelProjection
	{
		_MathV2::Matrix4x4<float> Model;
	};

	struct Material_InputLayout
	{
		VertexInput VertexInput;

		std::vector<ShaderParameter> ShaderParameters;

		DescriptorSetLayout DescriptorSetLayout;
		DescriptorPool DescriptorPool;
	};

	struct Material_RenderingSpecifications
	{
		_Core::OptionalT<VkPrimitiveTopology> PrimitiveTopologyOverride;
		GraphicsPipelie_DepthTest_Specification Depth;
	};

	struct MaterialUniqueKey
	{
		std::string VertexShaderPath;
		std::string FragmentShaderPath;
	};

	size_t MaterialUniqueKey_buildHash(MaterialUniqueKey* p_materialUniqueKey);

	typedef size_t MaterialRenderingOrder;
	extern size_t MaterialRenderingOrder_opaque;
	extern size_t MaterialRenderingOrder_gizmo;

	struct Material
	{
		struct FinalDrawObjects
		{
			VkPipelineLayout PipelineLayout;
			GraphicsPipeline GraphicsPipeline;
			MaterialDrawFn MaterialDrawFn;
		};

		MaterialUniqueKey MaterialUniqueKey;
		MaterialRenderingOrder RenderingOrder;

		Material_GlobalResources GlobalResources;
		Material_ExternalResources ExternalResources;

		Material_InputLayout InputLayout;
		Material_RenderingSpecifications RenderingSpecifications;

		FinalDrawObjects FinalDrawObjects;
	};

	struct MaterialAllocInfo
	{
		size_t RenderingOrder;
		std::string VertexShader;
		std::string FragmentShader;
		VertexInput VertexInput;

		/** OPTIONAL - Default value is VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		_Core::OptionalT<VkPrimitiveTopology> PrimitiveTopologyOverride;
		GraphicsPipelie_DepthTest_Specification DepthSpecification;

		MaterialDrawFn MaterialDrawFn;
		std::vector<ShaderParameter> ShaderParameters;
	};

	Material* Material_alloc(RenderInterface* p_renderInterface, MaterialAllocInfo* p_materialAllocInfo);
	void Material_free(Material** p_defaultMaterialV2, RenderInterface* p_renderInterface);
	void Material_reAllocGraphicsPipeline(Material* p_defaultMaterialV2, RenderInterface* p_renderInterface);
};