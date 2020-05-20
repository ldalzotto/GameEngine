#include "Extensions.h"

#include <stdexcept>
#include <cstring>

namespace _GameEngine::_Render::_Extensions
{
	/**
		@brief Query the Graphics API to get the supported extensions.
	*/
	std::vector<char*> getSupportedInstanceExtensionsV2()
	{
		uint32_t l_vulkanExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &l_vulkanExtensionCount, nullptr);

		std::vector<VkExtensionProperties> l_extensions(l_vulkanExtensionCount);
		std::vector<char*> l_extensionsName(l_vulkanExtensionCount);
		for (int i = 0; i < l_extensionsName.size(); i++) { l_extensionsName[i] = new char[VK_MAX_EXTENSION_NAME_SIZE]; }

		vkEnumerateInstanceExtensionProperties(nullptr, &l_vulkanExtensionCount, l_extensions.data());

		for (int i = 0; i < l_extensions.size(); i++)
		{
			std::strcpy(l_extensionsName[i], l_extensions[i].extensionName);
		}

		return l_extensionsName;
	};

	std::vector<char*> getSupportedDeviceExtensions(VkPhysicalDevice p_device)
	{
		uint32_t l_vulkanExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(p_device, nullptr, &l_vulkanExtensionCount, nullptr);

		std::vector<VkExtensionProperties> l_extensions(l_vulkanExtensionCount);
		std::vector<char*> l_extensionsName(l_vulkanExtensionCount);
		for (int i = 0; i < l_extensionsName.size(); i++) { l_extensionsName[i] = new char[VK_MAX_EXTENSION_NAME_SIZE]; }

		vkEnumerateDeviceExtensionProperties(p_device, nullptr, &l_vulkanExtensionCount, l_extensions.data());

		for (int i = 0; i < l_extensions.size(); i++)
		{
			std::strcpy(l_extensionsName[i], l_extensions[i].extensionName);
		}

		return l_extensionsName;
	};

	void populateRequiredExtensions(std::vector<char*>* p_requiredExtensions, bool p_includeDebugExtensions)
	{
		if (p_includeDebugExtensions)
		{
			p_requiredExtensions->push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
	};

	bool checkPresenceOrRequiredExtensions(std::vector<char*>& p_requiredExtensions, std::vector<char*>& p_supportedExtensions)
	{
		for (auto&& l_requiredExtension : p_requiredExtensions)
		{
			bool l_requiredExtensionPresent = false;
			for (auto&& l_supportedExtension : p_supportedExtensions)
			{
				if (std::strcmp(l_requiredExtension, l_supportedExtension) == 0)
				{
					l_requiredExtensionPresent = true;
					break;
				}
			}
			if (!l_requiredExtensionPresent)
			{
				return false;
			}
		}
		return true;
	};

	void checkPresenceOfRequiredInstanceExtensions(std::vector<char*>& p_requiredExtensions)
	{
		if (!checkPresenceOrRequiredExtensions(p_requiredExtensions, getSupportedInstanceExtensionsV2()))
		{
			throw std::runtime_error("The graphics API doesn't support the required Window extensions.");
		}
	};

	bool checkPresenceOfRequiredDeviceExtensions(std::vector<char*>& p_requiredExtensions, VkPhysicalDevice  p_device)
	{
		return checkPresenceOrRequiredExtensions(p_requiredExtensions, getSupportedDeviceExtensions(p_device));
	};
}