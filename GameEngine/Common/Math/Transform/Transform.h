#pragma once

#include "Math/Vector/Vector.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Quaternion/Quaternion.h"

#include "DataStructures/Specifications/VectorT.hpp"

namespace _GameEngine::_Math
{
	struct Transform
	{
		Transform* Parent;
		_Core::VectorT<Transform*> Childs;

		bool MatricesMustBeRecalculated;
		bool UserFlag_HasChanged;
		
		_Math::Vector3f LocalPosition;
		_Math::Quaternionf LocalRotation;
		_Math::Vector3f LocalScale;

		/** This matrix will always be relative to the top level Transform (a Transform without parent). */
		_Math::Matrix4x4f LocalToWorldMatrix;
	};

	void Transform_addChild(Transform* p_transform, Transform* p_newChild);
	void Transform_detachParent(Transform* p_transform);

	void Transform_markMatricsForRecalculation(Transform* p_transform);

	void Transform_setLocalPosition(Transform* p_transform, _Math::Vector3f& p_localPosition);
	void Transform_setLocalRotation(Transform* p_transform, _Math::Quaternionf& p_localRotation);
	void Transform_setLocalScale(Transform* p_transform, _Math::Vector3f& p_localScale);

	void Transform_setWorldPosition(Transform* p_transform, _Math::Vector3f& p_worldPosition);
	void Transform_addToWorldPosition(Transform* p_transform, _Math::Vector3f& p_worldPosition_delta);

	_Math::Matrix4x4f Transform_getLocalToWorldMatrix(Transform* p_transform);
	_Math::Matrix4x4f* Transform_getLocalToWorldMatrix_ref(Transform* p_transform);

	_Math::Matrix4x4f Transform_getWorldToLocalMatrix(Transform* p_transform);

	_Math::Matrix4x4f Transform_calculateMatrixToProjectFromTransformToAnother(Transform* p_source, Transform* p_target);

	_Math::Vector3f Transform_getWorldPosition(Transform* p_transform);
	_Math::Quaternionf Transform_getWorldRotation(Transform* p_transform);
	_Math::Vector3f Transform_getWorldScale(Transform* p_transform);

	_Math::Vector3f Transform_getRight(Transform* p_transform);
	_Math::Vector3f Transform_getUp(Transform* p_transform);
	_Math::Vector3f Transform_getForward(Transform* p_transform);

	_Math::Vector3f Transform_getUp_worldSpace(Transform* p_transform);
	_Math::Vector3f Transform_getForward_worldSpace(Transform* p_transform);

	void Transform_alloc(Transform* p_transform);
	void Transform_free(Transform* p_transform);
}