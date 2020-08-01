#pragma once

#include "v2/Matrix/Matrix.hpp"
#include "v2/Vector/Vector.hpp"
#include "v2/Quaternion/Quaternion.hpp"

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
		_MathV2::Quaternion<float> LocalRotation;
		_MathV2::Vector3<float> LocalScale;

		/** This matrix will always be relative to the top level Transform (a Transform without parent). */
		_MathV2::Matrix4x4<float> LocalToWorldMatrix;
	};
	 
}