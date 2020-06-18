#include "ShaderParameterKeys.h"
#include <functional>

namespace _GameEngine::_Render
{
	size_t ShaderParameterKey_buildHash(ShaderParameterKey* p_key)
	{
		return std::hash<std::string>()(*p_key);
	};

	ShaderParameterKey MATERIALINSTANCE_MESH_KEY = "Mesh";
	ShaderParameterKey MATERIALINSTANCE_GIZMOMESH_KEY = "GizmoMesh";
	ShaderParameterKey MATERIALINSTANCE_TEXTURE_KEY = "Texture";
	ShaderParameterKey MATERIALINSTANCE_MODEL_BUFFER = "Model";
}