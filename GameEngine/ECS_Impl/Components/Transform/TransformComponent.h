#pragma once

#include "Math/Transform/Transform.h"

#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformComponentType;

	struct TransformComponent
	{
		_ECS::ComponentHeader ComponentHeader;
		_Math::Transform Transform;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<TransformComponent>()
	{
		return &TransformComponentType;
	}

	struct TransformInitInfo
	{
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;
	};

	void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
	TransformComponent* TransformComponent_castFromTransform(_Math::Transform* p_transform);
}