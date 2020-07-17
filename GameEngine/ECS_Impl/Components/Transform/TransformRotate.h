
#include "ECS/ComponentT.hpp"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformRotateType;

	struct TransformRotate
	{
		float Speed;
		_Math::Vector3f Axis;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<TransformRotate>()
	{
		return &TransformRotateType;
	};
}