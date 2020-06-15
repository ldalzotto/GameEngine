#pragma once

#include <string>

namespace _GameEngine::_Render
{
	typedef std::string ShaderParameterKey;

	size_t ShaderParameterKey_buildHash(ShaderParameterKey* p_key);

	extern ShaderParameterKey MATERIALINSTANCE_MESH_KEY;
	extern ShaderParameterKey MATERIALINSTANCE_TEXTURE_KEY;
	extern ShaderParameterKey MATERIALINSTANCE_MODEL_BUFFER;


}