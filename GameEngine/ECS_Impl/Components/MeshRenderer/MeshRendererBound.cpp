#include "MeshRendererBound.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererBoundType = "MeshRendererBound";

	template<>
	inline ComponentType* extractComponentType<MeshRendererBound>() { return &MeshRendererBoundType; };
}