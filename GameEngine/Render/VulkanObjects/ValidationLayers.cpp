#include "ValidationLayers.h"

#include "Includes/VulkanIncludes.h"
#include <stdexcept>
#include <cstring>

namespace _GameEngine::_Render
{
		void ValidationLayer_init(ValidationLayers* p_validationLayers)
		{

#ifndef NDEBUG
			p_validationLayers->EnableValidationLayers = true;
#else
			p_validationLayers->EnableValidationLayers = false;
#endif

			p_validationLayers->ValidationLayers = std::vector<const char*>{
				"VK_LAYER_KHRONOS_validation"
			};
		}

		void ValidationLayer_checkSupport(ValidationLayers* p_validationLayers) 
		{
			if (p_validationLayers->EnableValidationLayers) 
			{
				uint32_t l_layerCount;
				vkEnumerateInstanceLayerProperties(&l_layerCount, nullptr);
				std::vector<VkLayerProperties> l_layerPorperties(l_layerCount);
				vkEnumerateInstanceLayerProperties(&l_layerCount, l_layerPorperties.data());

				for (int i = 0; i < p_validationLayers->ValidationLayers.size(); i++)
				{
					bool l_validationLayerSupported = false;

					for (auto&& l_layerPropery : l_layerPorperties)
					{
						if (std::strcmp(p_validationLayers->ValidationLayers[i], l_layerPropery.layerName) == 0)
						{
							l_validationLayerSupported = true;
							break;
						}
					}

					if (!l_validationLayerSupported)
					{
						throw std::runtime_error("validation layers requested, but not available!");
					}
				}
			}
		}

} // namespace _GameEngine