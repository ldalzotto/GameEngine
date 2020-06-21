#include "MaterialResourceProvider.h"

#include "MyLog/MyLog.h"
#include <stdexcept>

#include "Math/Vector/Vector.h"

#include "Shader/ShaderParameterKeys.h"
#include "Materials/MaterialInstanceContainer.h"
#include "RenderInterface.h"
#include "Texture/TextureSamplers.h"

namespace _GameEngine::_Render
{
	_Math::Vector4f Material_ColorDefaultValue = { 1.0f, 1.0f, 1.0f, 1.0f };

	Material* materialResourceProvider_allocateMaterial(MaterialUniqueKey* p_key, RenderInterface* p_renderInterface);

	Material* MaterialResourceProvider_UseResource(MaterialResourceProvider* p_materialResourceProvider, MaterialUniqueKey* p_key)
	{
		size_t l_hash = MaterialUniqueKey_buildHash(p_key);

		if (!p_materialResourceProvider->MaterialResources.contains(l_hash))
		{

			Material* l_material = materialResourceProvider_allocateMaterial(p_key, p_materialResourceProvider->RenderInterface);
			MaterialInstanceContainer_addMaterial(p_materialResourceProvider->RenderInterface->MaterialInstanceContainer, l_material);

			MaterialWithCounter l_materialWithCounter{};

			l_materialWithCounter.Material = l_material;
			l_materialWithCounter.UsageCounter.UsageCount = 0;
			p_materialResourceProvider->MaterialResources.emplace(l_hash, l_materialWithCounter);
		}

		MaterialWithCounter* l_resourceWithCounter = &p_materialResourceProvider->MaterialResources.at(l_hash);
		_Utils::UsageCounter_use(&l_resourceWithCounter->UsageCounter);
		return l_resourceWithCounter->Material;
	};

	void MaterialResourceProvider_ReleaseResource(MaterialResourceProvider* p_materialResourceProvider, MaterialUniqueKey* p_key)
	{
		size_t l_hash = MaterialUniqueKey_buildHash(p_key);
		MaterialWithCounter* l_resourceWithCounter = &p_materialResourceProvider->MaterialResources.at(l_hash);
		_Utils::UsageCounter_release(&l_resourceWithCounter->UsageCounter);
		if (l_resourceWithCounter->UsageCounter.UsageCount == 0)
		{
			MaterialInstanceContainer_removeMaterial(p_materialResourceProvider->RenderInterface->MaterialInstanceContainer, l_resourceWithCounter->Material);
			Material_free(&l_resourceWithCounter->Material, p_materialResourceProvider->RenderInterface);
			p_materialResourceProvider->MaterialResources.erase(l_hash);
		}
	};

	void MaterialResourceProvider_Clear(MaterialResourceProvider* p_materialResourceProvider)
	{
		if (p_materialResourceProvider->MaterialResources.size() > 0)
		{
#ifndef NDEBUG
			std::string l_textureResourcesNotDisposed = "[";
			for (auto&& l_textureResourceEntry : p_materialResourceProvider->MaterialResources)
			{
				l_textureResourcesNotDisposed += l_textureResourceEntry.second.Material->MaterialUniqueKey.FragmentShaderPath;
				l_textureResourcesNotDisposed += ",";
				l_textureResourcesNotDisposed += l_textureResourceEntry.second.Material->MaterialUniqueKey.VertexShaderPath;
				l_textureResourcesNotDisposed += ",";
			}
			l_textureResourcesNotDisposed += "]";
			MYLOG_PUSH(p_materialResourceProvider->RenderInterface->MyLog, _Log::WARN, (char*)("TextureResourceProvider : Potential memory Leak. Texture resource " + l_textureResourcesNotDisposed + " wasn't disposed.").c_str());
#endif

			for (auto&& l_textureResourceEntry : p_materialResourceProvider->MaterialResources)
			{
				Material_free(&l_textureResourceEntry.second.Material, p_materialResourceProvider->RenderInterface);
			}
			p_materialResourceProvider->MaterialResources.clear();
		}
	};

	Material* materialResourceProvider_allocateMaterial(MaterialUniqueKey* p_key, RenderInterface* p_renderInterface)
	{
		if (p_key->VertexShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialVertex.spv"
			&& p_key->FragmentShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialFragment.spv")
		{

			MaterialAllocInfo l_materialAllocInfo{};
			l_materialAllocInfo.RenderingOrder = MaterialRenderingOrder_opaque;
			l_materialAllocInfo.VertexShader = p_key->VertexShaderPath;
			l_materialAllocInfo.FragmentShader = p_key->FragmentShaderPath;
			l_materialAllocInfo.MaterialDrawFn = MaterialDrawFn_meshDraw;
			l_materialAllocInfo.UseDepthBuffer = true;
			VertexInput_buildInput(&l_materialAllocInfo.VertexInput);

			ShaderParameter l_modelMatrixShaderParameter{};
			ShaderParameter_alloc(&l_modelMatrixShaderParameter, ShaderParameterType::UNIFORM_BUFFER, MATERIALINSTANCE_MODEL_BUFFER);

			UniformBufferParameter* l_modelMatrixParameter = (UniformBufferParameter*)l_modelMatrixShaderParameter.Parameter;
			l_modelMatrixParameter->Binding = 0;
			l_modelMatrixParameter->BufferSize = sizeof(ModelProjection);
			l_modelMatrixParameter->StageFlag = VK_SHADER_STAGE_VERTEX_BIT;
			l_modelMatrixShaderParameter.DescriptorSetLayoutBinding = UniformBufferParameter_buildLayoutBinding(l_modelMatrixParameter);


			ShaderParameter l_imageSamplerShaderParameter{};
			ShaderParameter_alloc(&l_imageSamplerShaderParameter, ShaderParameterType::IMAGE_SAMPLER, MATERIALINSTANCE_TEXTURE_KEY);

			ImageSampleParameter* l_imageSampleParameter = (ImageSampleParameter*)l_imageSamplerShaderParameter.Parameter;
			l_imageSampleParameter->Binding = 1;
			l_imageSampleParameter->TextureSampler = p_renderInterface->TextureSamplers->DefaultSampler;
			l_imageSampleParameter->StageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
			l_imageSamplerShaderParameter.DescriptorSetLayoutBinding = ImageSampleParameter_buildLayoutBinding(l_imageSampleParameter);

			ShaderParameter l_colorParameter{};
			ShaderParameter_alloc(&l_colorParameter, ShaderParameterType::UNIFORM_BUFFER, MATERIALINSTANCE_COLOR);

			UniformBufferParameter* l_colorParameterBufer = (UniformBufferParameter*)l_colorParameter.Parameter;
			l_colorParameterBufer->Binding = 2;
			l_colorParameterBufer->BufferSize = sizeof(_Math::Vector4f);
			l_colorParameterBufer->StageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
			l_colorParameterBufer->DefaultValue = &Material_ColorDefaultValue;
			l_colorParameter.DescriptorSetLayoutBinding = UniformBufferParameter_buildLayoutBinding(l_colorParameterBufer);

			l_materialAllocInfo.ShaderParameters.emplace_back(l_modelMatrixShaderParameter);
			l_materialAllocInfo.ShaderParameters.emplace_back(l_imageSamplerShaderParameter);
			l_materialAllocInfo.ShaderParameters.emplace_back(l_colorParameter);

			return Material_alloc(p_renderInterface, &l_materialAllocInfo);
		}
		else if (p_key->VertexShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/EditorVertex.spv"
			&& p_key->FragmentShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/EditorFragment.spv")
		{
			MaterialAllocInfo l_materialAllocInfo{};
			l_materialAllocInfo.RenderingOrder = MaterialRenderingOrder_opaque;
			l_materialAllocInfo.VertexShader = p_key->VertexShaderPath;
			l_materialAllocInfo.FragmentShader = p_key->FragmentShaderPath; 
			l_materialAllocInfo.MaterialDrawFn = MaterialDrawFn_meshDraw;
			l_materialAllocInfo.UseDepthBuffer = true;
			VertexInput_buildInput(&l_materialAllocInfo.VertexInput);

			ShaderParameter l_modelMatrixShaderParameter{};
			ShaderParameter_alloc(&l_modelMatrixShaderParameter, ShaderParameterType::UNIFORM_BUFFER, MATERIALINSTANCE_MODEL_BUFFER);

			UniformBufferParameter* l_modelMatrixParameter = (UniformBufferParameter*)l_modelMatrixShaderParameter.Parameter;
			l_modelMatrixParameter->Binding = 0;
			l_modelMatrixParameter->BufferSize = sizeof(ModelProjection);
			l_modelMatrixParameter->StageFlag = VK_SHADER_STAGE_VERTEX_BIT;
			l_modelMatrixShaderParameter.DescriptorSetLayoutBinding = UniformBufferParameter_buildLayoutBinding(l_modelMatrixParameter);


			ShaderParameter l_imageSamplerShaderParameter{};
			ShaderParameter_alloc(&l_imageSamplerShaderParameter, ShaderParameterType::IMAGE_SAMPLER, MATERIALINSTANCE_TEXTURE_KEY);

			ImageSampleParameter* l_imageSampleParameter = (ImageSampleParameter*)l_imageSamplerShaderParameter.Parameter;
			l_imageSampleParameter->Binding = 1;
			l_imageSampleParameter->TextureSampler = p_renderInterface->TextureSamplers->DefaultSampler;
			l_imageSampleParameter->StageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;
			l_imageSamplerShaderParameter.DescriptorSetLayoutBinding = ImageSampleParameter_buildLayoutBinding(l_imageSampleParameter);

			l_materialAllocInfo.ShaderParameters.emplace_back(l_modelMatrixShaderParameter);
			l_materialAllocInfo.ShaderParameters.emplace_back(l_imageSamplerShaderParameter);

			return Material_alloc(p_renderInterface, &l_materialAllocInfo);
		}
		else if (p_key->VertexShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/GizmoVertex.spv"
			&& p_key->FragmentShaderPath == "E:/GameProjects/GameEngine/Assets/Shader/out/GizmoFragment.spv")
		{
			MaterialAllocInfo l_materialAllocInfo{};
			l_materialAllocInfo.RenderingOrder = MaterialRenderingOrder_gizmo;
			l_materialAllocInfo.VertexShader = p_key->VertexShaderPath;
			l_materialAllocInfo.FragmentShader = p_key->FragmentShaderPath;
			l_materialAllocInfo.MaterialDrawFn = MaterialDrawFn_gizmoDraw;
			l_materialAllocInfo.UseDepthBuffer = false;
			VertexInput_buildGizmoInfo(&l_materialAllocInfo.VertexInput);
			l_materialAllocInfo.PrimitiveTopologyOverride.HasValue = true;
			l_materialAllocInfo.PrimitiveTopologyOverride.Value = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			return Material_alloc(p_renderInterface, &l_materialAllocInfo);
		}
		else
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Cannot build material."));
		}

	};
}