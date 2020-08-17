#pragma once

#include "v2/Matrix/Matrix.hpp"
#include "v2/Vector/Vector.hpp"
extern "C"
{
#include "v2/_interface/QuaternionC_def.h"
}

#include "DataStructures/Specifications/VectorT.hpp"

namespace _MathV2
{
	struct Transform;
	using TransformHandle = Transform*;

	struct Transform
	{
		TransformHandle Parent;
		_Core::VectorT<TransformHandle> Childs;

		bool MatricesMustBeRecalculated;
		bool UserFlag_HasChanged;
		
		_MathV2::Vector3<float> LocalPosition;
		QUATERNION4F LocalRotation;
		_MathV2::Vector3<float> LocalScale;

		/** This matrix will always be relative to the top level Transform (a Transform without parent). */
		_MathV2::Matrix4x4<float> LocalToWorldMatrix;
	};
	 
}