#pragma once

#include <vulkan/vulkan.h>

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct MaterialInstance;
}

namespace _GameEngine::_Render
{
	typedef void (*MaterialDrawFn)(VkCommandBuffer, MaterialInstance*, RenderInterface*);

	void MaterialDrawFn_meshDraw(VkCommandBuffer, MaterialInstance*, RenderInterface*);
	void MaterialDrawFn_gizmoDraw(VkCommandBuffer, MaterialInstance*, RenderInterface*);
}