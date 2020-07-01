#include "TransformRotate.h"

namespace _GameEngine::_ECS
{
	ComponentType TransformRotateType = "TransformRotate";
	template<>
	inline ComponentType* extractComponentType<TransformRotate>() { return &TransformRotateType; };
}