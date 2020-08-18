#pragma once

extern "C"
{
#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/QuaternionC.h"
}
#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformComponentType;

	struct TransformComponent
	{
		_ECS::ComponentHeaderT<TransformComponent> ComponentHeader;
		TRANSFORM Transform;
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
		_MathV2::Vector3<float> LocalScale = { 1.0f, 1.0f, 1.0f };
	};

	void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
	TransformComponent* TransformComponent_castFromTransform(TRANSFORM_PTR p_transform);
}