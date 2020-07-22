#pragma once

#include "Math/Transform/Transform.h"

#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/ComponentT.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformComponentType;

	struct TransformComponent
	{
		_ECS::ComponentHeaderT<TransformComponent> ComponentHeader;
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
		_Math::Quaternionf LocalRotation = _Math::Quaternionf_identity();
		_Math::Vector3f LocalScale = {1.0f, 1.0f, 1.0f};
	};

	void TransformComponent_init(TransformComponent* p_component, TransformInitInfo* p_transformInitInfo);
	TransformComponent* TransformComponent_castFromTransform(_Math::Transform* p_transform);
}