#pragma once

#include <unordered_map>

#include "Render/Shader/Shader.h"

namespace _GameEngine::_Render
{
	struct ShaderContainer
	{
		std::unordered_map<ShaderUniqueID, Shader*> Shaders;
	};

}