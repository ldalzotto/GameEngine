#pragma once

#include <string>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
}

namespace _GameEngine::_Render
{
	struct TextureInfo
	{
		uint32_t MipLevels;
		uint32_t ArrayLayers;
		VkFormat Format;
		uint32_t Width;
		uint32_t Height;
	};

	struct Texture
	{
		TextureInfo TextureInfo;
		VkImage Texture;
		VkDeviceMemory TextureMemory;
	};

	struct TextureLoadInfo
	{
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	void Texture_load(Texture* p_texture,const std::string& l_texturePath, TextureLoadInfo* l_textureLoadInfo);
}