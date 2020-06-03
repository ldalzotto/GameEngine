#pragma once

#include <vector>
#include <string>

namespace _GameEngine::_Render
{
		struct ValidationLayers 
		{
			std::vector<const char*> ValidationLayers;
			bool EnableValidationLayers;
		};

		void ValidationLayer_init(ValidationLayers* p_validationLayers);

		void ValidationLayer_checkSupport(ValidationLayers* p_validationLayers);

} // namespace _GameEngine 