#pragma once

#include "Window/Window.h"
#include "vulkan/vulkan.h"
#include "ValidationLayers.h"

namespace _GameEngine::_Render
{
	struct RenderDebug
	{
		VkDebugUtilsMessengerEXT DebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT PfnCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT PfnDestroyDebugUtilsMessengerEXT;
	};

	struct Render
	{
		Window Window;
		
		VkInstance Instance;
		_ValidationLayers::ValidationLayers ValidationLayers;
		RenderDebug RenderDebug;
	};

	Render* alloc();
	void free(Render* p_render);

	void render(Render* p_render);

} // namespace _GameEngine::_Render
