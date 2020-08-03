#pragma once

#include "Transform.hpp"

namespace _MathV2
{
	struct TransformM
	{
		static void addChild(TransformHandle p_transform, TransformHandle p_newChild);
		static void detachParent(TransformHandle p_transform);

		static void markMatricsForRecalculation(Transform* p_transform);

		static void setLocalPosition(Transform* p_transform, const _MathV2::Vector3<float>* p_localPosition);
		static void setLocalRotation(Transform* p_transform, const _MathV2::Quaternion<float>* p_localRotation);
		static void setLocalScale(Transform* p_transform, const _MathV2::Vector3<float>* p_localScale);

		static void setWorldPosition(Transform* p_transform, const _MathV2::Vector3<float>* p_worldPosition);
		static void addToWorldPosition(Transform* p_transform, const _MathV2::Vector3<float>* p_worldPosition_delta);
		static void setWorldRotation(Transform* p_transform, const _MathV2::Quaternion<float>* p_worldRotation);
		static void setWorldScale(Transform* p_transform, const _MathV2::Vector3<float>* p_worldScale);

		static _MathV2::Matrix4x4<float>* getLocalToWorldMatrix(Transform* p_transform, _MathV2::Matrix4x4<float>* p_out);
		static _MathV2::Matrix4x4<float>* getWorldToLocalMatrix(Transform* p_transform, _MathV2::Matrix4x4<float>* p_out);

		static _MathV2::Matrix4x4<float>* calculateMatrixToProjectFromTransformToAnother(Transform* p_source, Transform* p_target, _MathV2::Matrix4x4<float>* p_out);

		static _MathV2::Vector3<float>* getWorldPosition(Transform* p_transform, _MathV2::Vector3<float>* p_out);
		static _MathV2::Quaternion<float>* getWorldRotation(Transform* p_transform, _MathV2::Quaternion<float>* p_out);
		static _MathV2::Vector3<float>* getWorldScaleFactor(Transform* p_transform, _MathV2::Vector3<float>* p_out);

		static _MathV2::Vector3<float>* getRight(Transform* p_transform, _MathV2::Vector3<float>* p_out);
		static _MathV2::Vector3<float>* getUp(Transform* p_transform, _MathV2::Vector3<float>* p_out);
		static _MathV2::Vector3<float>* getForward(Transform* p_transform, _MathV2::Vector3<float>* p_out);

		static void alloc(TransformHandle p_transform);
		static void free(TransformHandle p_transform);
	};
}