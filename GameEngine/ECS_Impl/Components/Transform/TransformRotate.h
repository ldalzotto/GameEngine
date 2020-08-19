
#include "ECS/ComponentT.hpp"

extern "C"
{
#include "v2/_interface/VectorC_def.h"
}

namespace _GameEngine::_ECS
{
	extern ComponentType TransformRotateType;

	struct TransformRotate
	{
		_ECS::ComponentHeaderT<TransformRotate> ComponentHeader;
		float Speed;
		VECTOR3F Axis;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<TransformRotate>()
	{
		return &TransformRotateType;
	};
}