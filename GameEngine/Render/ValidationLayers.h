#pragma once

#include <vector>
#include <string>

namespace _GameEngine
{
	namespace _ValidationLayers {

		struct ValidationLayers 
		{
			std::vector<const char*> ValidationLayers;
			bool EnableValidationLayers;
		};

		void init(ValidationLayers* p_validationLayers);

		void checkValidationLayerSupport(ValidationLayers* p_validationLayers);
	} // namespace _ValidationLayers

} // namespace _GameEngine 