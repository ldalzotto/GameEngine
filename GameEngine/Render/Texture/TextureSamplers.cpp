#include "TextureSamplers.h"

#include <stdexcept>

extern "C"
{
#include "Log/LogFormatting.h"
}

#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	void TextureSamplers_alloc(TextureSamplers* p_textureSamplers, Device* p_device)
	{
		VkSamplerCreateInfo l_samplerCreateInfo{};
		l_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		l_samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		l_samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		l_samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		l_samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		l_samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		if (p_device->PhysicalDevice.DeviceAvailableFeatures.samplerAnisotropy)
		{
			l_samplerCreateInfo.anisotropyEnable = VK_TRUE;
			l_samplerCreateInfo.maxAnisotropy = 16.0f;
		}
		else
		{
			l_samplerCreateInfo.anisotropyEnable = VK_FALSE;
			l_samplerCreateInfo.maxAnisotropy = 1.0f;
		}

		l_samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		l_samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		l_samplerCreateInfo.compareEnable = VK_FALSE;
		l_samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		l_samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		l_samplerCreateInfo.mipLodBias = 0.0f;
		l_samplerCreateInfo.minLod = 0.0f;
		l_samplerCreateInfo.maxLod = 0.0f;

		if (vkCreateSampler(p_device->LogicalDevice.LogicalDevice, &l_samplerCreateInfo, nullptr, &p_textureSamplers->DefaultSampler) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create texture sampler!"));	
		}
	};
	
	void TextureSamplers_free(TextureSamplers* p_textureSamplers, Device* p_device)
	{
		vkDestroySampler(p_device->LogicalDevice.LogicalDevice, p_textureSamplers->DefaultSampler, nullptr);
	};
}