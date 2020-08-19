#pragma once

extern "C"
{
#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/QuaternionC.h"
#include "v2/_interface/VectorC_def.h"
}
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
		VECTOR3F LocalPosition;
		QUATERNION4F LocalRotation = QUATERNION4F_IDENTITY;
		VECTOR3F LocalScale = { 1.0f, 1.0f, 1.0f };
	};

	void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
	TransformComponent* TransformComponent_castFromTransform(TRANSFORM_PTR p_transform);
}