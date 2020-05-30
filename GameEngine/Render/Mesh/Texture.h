#pragma once

#include <string>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct Texture
	{
		VkImage Texture;
		VkDeviceMemory TextureMemory;
	};

	struct TextureLoadInfo
	{
		Device* Device;
	};

	void Texture_load(Texture* p_texture, std::string& l_texturePath, TextureLoadInfo* l_textureLoadInfo);
}