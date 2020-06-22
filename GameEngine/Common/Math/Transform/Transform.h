#pragma once

#include "Math/Vector/Vector.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Quaternion/Quaternion.h"

#include "DataStructures/VectorT.h"

namespace _GameEngine::_Math
{
	struct Transform
	{
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;

		_Math::Matrix4x4f LocalToWorldMatrix;
	};
}