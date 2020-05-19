#include "Render.h"

#include "Log/Log.h"

#include <stdexcept>
#include <vector>

namespace _GameEngine
{
	void Render_initializeVulkan(Render* p_render);
	void Render_freeVulkan(Render* p_render);

	/// Graphics extensions check
	void Render_checkPresenceOfRequiredWindowExtensions();
	std::vector<std::string> Render_getSupportedExtensions();
	/// END

	Render* Render_alloc()
	{
		Render* l_render = new Render();
		Window_init(&l_render->Window);
		Render_initializeVulkan(l_render);
		return l_render;
	};

	void Render_free(Render* p_render)
	{
		Render_freeVulkan(p_render);
		delete p_render;
	};

	void Render_initializeVulkan(Render* p_render)
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "GameEngine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		char** l_extension = nullptr;
		uint32_t l_extensionCount = 0;
		Window_get_instanceExtension(&l_extension, &l_extensionCount);

		createInfo.enabledExtensionCount = l_extensionCount;
		createInfo.ppEnabledExtensionNames = l_extension;

		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &p_render->Instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance.");
		}

#ifndef NDEBUG
		Render_checkPresenceOfRequiredWindowExtensions();
#endif // !NDEBUG

	}

	void Render_freeVulkan(Render* p_render) 
	{
		vkDestroyInstance(p_render->Instance, nullptr);
	}

	/// Graphics extensions check

	void Render_checkPresenceOfRequiredWindowExtensions() 
	{
		std::vector<std::string> l_supportedExtensions = Render_getSupportedExtensions();
		std::vector<std::string> l_requiredExtensions = Window_getRequiredExtensions();
		for (auto&& l_requiredExtension : l_requiredExtensions) 
		{
			if (std::find(l_supportedExtensions.begin(), l_supportedExtensions.end(), l_requiredExtension) == l_supportedExtensions.end()) 
			{
				throw std::runtime_error("The graphics API doesn't support the required Window extensions : " + l_requiredExtension);
			}
		}
	}

	std::vector<std::string> Render_getSupportedExtensions() 
	{
		/// Extenstions debug log
		uint32_t l_vulkanExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &l_vulkanExtensionCount, nullptr);

		std::vector<VkExtensionProperties> l_extensions(l_vulkanExtensionCount);
		std::vector<std::string> l_extensionsName(l_vulkanExtensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &l_vulkanExtensionCount, l_extensions.data());

		for (int i = 0; i < l_extensions.size(); i++) 
		{
			l_extensionsName.at(i) = std::string(l_extensions[i].extensionName);
		}

		return l_extensionsName;
	};

	/// END

	void Render_render(Render* p_render)
	{

	};

} // namespace _GameEngine