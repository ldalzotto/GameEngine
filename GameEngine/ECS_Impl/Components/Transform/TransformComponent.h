#pragma once

#include "Math/Transform/Transform.h"

#include "DataStructures/Specifications/VectorT.hpp"
#include "ECS/Component.h"

extern "C"
{
#include "Functional/Callback/Callback.h"
}

namespace _GameEngine::_ECS
{
	extern ComponentType TransformComponentType;

	struct TransformComponent
	{
		_Math::Transform Transform;
		Callback OnComponentDetached;
	};

	struct TransformInitInfo
	{
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;
	};

	void TransformComponent_init(Component* p_component, TransformInitInfo* p_transformInitInfo);

}