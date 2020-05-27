#pragma once

#include <unordered_map>

#include "Render/GraphcisPipeline/GraphicsPipeline.h"

namespace _GameEngine::_Render
{

	struct GraphcisPipelineContainer
	{
		std::unordered_map<GraphicsPipelineKey, GraphicsPipeline*, GraphicsPipelineKeyHasher> GraphicsPipelines;
	};
}