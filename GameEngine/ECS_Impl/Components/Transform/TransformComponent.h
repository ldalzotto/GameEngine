#pragma once

#include "v2/Transform/Transform.hpp"
extern "C"
{
#include "v2/_interface/QuaternionC.h"
}
#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformComponentType;

	struct TransformComponent
	{
		_ECS::ComponentHeaderT<TransformComponent> ComponentHeader;
		_MathV2::Transform Transform;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<TransformComponent>()
	{
		return &TransformComponentType;
	}

	struct TransformInitInfo
	{
		_MathV2::Vector3<float> LocalPosition;
		QUATERNION4F LocalRotation = QUATERNION4F_IDENTITY;
		_MathV2::Vector3<float> LocalScale = {1.0f, 1.0f, 1.0f};
	};

	void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
	TransformComponent* TransformComponent_castFromTransform(_MathV2::Transform* p_transform);
}