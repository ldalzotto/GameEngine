#pragma once

#include "v2/Matrix/Matrix.hpp"
#include "v2/Vector/Vector.hpp"
#include "v2/Quaternion/Quaternion.hpp"

#include "DataStructures/Specifications/VectorT.hpp"

namespace _GameEngine::_Math
{
	struct Transform
	{
		Transform* Parent;
		_Core::VectorT<Transform*> Childs;

		bool MatricesMustBeRecalculated;
		bool UserFlag_HasChanged;
		
		_MathV2::Vector3<float> LocalPosition;
		_MathV2::Quaternion<float> LocalRotation;
		_MathV2::Vector3<float> LocalScale;

		/** This matrix will always be relative to the top level Transform (a Transform without parent). */
		_MathV2::Matrix4x4<float> LocalToWorldMatrix;
	};

	void Transform_addChild(Transform* p_transform, Transform* p_newChild);
	void Transform_detachParent(Transform* p_transform);

	void Transform_markMatricsForRecalculation(Transform* p_transform);

	void Transform_setLocalPosition(Transform* p_transform, _MathV2::Vector3<float>& p_localPosition);
	void Transform_setLocalRotation(Transform* p_transform, const _MathV2::Quaternion<float>& p_localRotation);
	void Transform_setLocalScale(Transform* p_transform, _MathV2::Vector3<float>& p_localScale);

	void Transform_setWorldPosition(Transform* p_transform, _MathV2::Vector3<float>& p_worldPosition);
	void Transform_addToWorldPosition(Transform* p_transform, _MathV2::Vector3<float>& p_worldPosition_delta);

	_MathV2::Matrix4x4<float> Transform_getLocalToWorldMatrix(Transform* p_transform);
	_MathV2::Matrix4x4<float>* Transform_getLocalToWorldMatrix_ref(Transform* p_transform);

	_MathV2::Matrix4x4<float> Transform_getWorldToLocalMatrix(Transform* p_transform);

	_MathV2::Matrix4x4<float> Transform_calculateMatrixToProjectFromTransformToAnother(Transform* p_source, Transform* p_target);

	_MathV2::Vector3<float> Transform_getWorldPosition(Transform* p_transform);
	_MathV2::Quaternion<float> Transform_getWorldRotation(Transform* p_transform);
	_MathV2::Vector3<float> Transform_getWorldScale(Transform* p_transform);

	_MathV2::Vector3<float> Transform_getRight(Transform* p_transform);
	_MathV2::Vector3<float> Transform_getUp(Transform* p_transform);
	_MathV2::Vector3<float> Transform_getForward(Transform* p_transform);

	_MathV2::Vector3<float> Transform_getRight_worldSpace(Transform* p_transform);
	_MathV2::Vector3<float> Transform_getUp_worldSpace(Transform* p_transform);
	_MathV2::Vector3<float> Transform_getForward_worldSpace(Transform* p_transform);

	void Transform_alloc(Transform* p_transform);
	void Transform_free(Transform* p_transform);
}