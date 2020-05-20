#pragma once

#include <vector>

/**
	@brief Extensions are the graphics API features.

	The Graphics API can be queryied to retrieve all the available extensions for the cuurent system
	(@see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkEnumerateInstanceExtensionProperties.html)
*/
namespace _GameEngine::_Render::_Extensions
{
	/**
		@brief Populates the @param p_requiredExtensions with additional extensions.

		@param p_includeDebugExtensions if true, additional extensions used to debug the Graphics API will be
										enabled.
	*/
	void populateRequiredExtensions(std::vector<char*>* p_requiredExtensions, bool p_includeDebugExtensions = false);

	/**
		@brief Compare the required extensions with the supported.

		@throw @see std::runtime_error if required extensions are not fulfilled.
	*/
	void checkPresenceOfRequiredWindowExtensions(std::vector<char*>& p_requiredExtensions);
}