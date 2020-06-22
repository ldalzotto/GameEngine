#pragma once

#include "Math/Vector/Vector.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Quaternion/Quaternion.h"

#include "DataStructures/VectorT.h"

namespace _GameEngine::_Math
{
	struct Transform
	{
		Transform* Parent;
		_Core::VectorT<Transform*> Childs;

		bool MatricesMustBeRecalculated;
		bool UserFlag_ChangesMade;
		
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;
		_Math::Matrix4x4f LocalToWorldMatrix;
	};

	void Transform_addChild(Transform* p_transform, Transform* p_newChild);
	void Transform_detachParent(Transform* p_transform);

	void Transform_markMatricsForRecalculation(Transform* p_transform);

	void Transform_setLocalPosition(Transform* p_transform, _Math::Vector3f& p_localPosition);
	void Transform_setLocalRotation(Transform* p_transform, _Math::Quaternionf& p_localRotation);
	void Transform_setLocalScale(Transform* p_transform, _Math::Vector3f& p_localScale);
	_Math::Matrix4x4f Transform_getLocalToWorldMatrix(Transform* p_transform, bool p_includeSelf = true);

	_Math::Vector3f Transform_getWorldPosition(Transform* p_transform);
	_Math::Quaternionf Transform_getWorldRotation(Transform* p_transform);
	_Math::Vector3f Transform_getWorldScale(Transform* p_transform);

	_Math::Vector3f Transform_getUp(Transform* p_transform);
	_Math::Vector3f Transform_getForward(Transform* p_transform);

	void Transform_alloc(Transform* p_transform);
	void Transform_free(Transform* p_transform);
}