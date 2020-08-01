
#include "ECS/ComponentT.hpp"
#include "v2/Vector/Vector.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformRotateType;

	struct TransformRotate
	{
		_ECS::ComponentHeaderT<TransformRotate> ComponentHeader;
		float Speed;
		_MathV2::Vector3<float> Axis;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<TransformRotate>()
	{
		return &TransformRotateType;
	};
}