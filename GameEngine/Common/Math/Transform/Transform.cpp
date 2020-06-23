#include "Transform.h"

#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Quaternion/QuaternionMath.h"

namespace _GameEngine::_Math
{
	bool Vector_transformComparator(Transform** left, Transform** right)
	{
		return *left == *right;
	}
}


namespace _GameEngine::_Math
{
	void transform_updateMatricesIfNecessary(Transform* p_transform);

	void Transform_addChild(Transform* p_transform, Transform* p_newChild)
	{
		if (p_newChild->Parent != p_transform)
		{
			if (p_newChild->Parent)
			{
				p_newChild->Parent->Childs.erase(Vector_transformComparator, &p_newChild);
			}

			p_newChild->Parent = p_transform;
			p_transform->Childs.push_back(&p_newChild);

			Transform_markMatricsForRecalculation(p_newChild);
		}
	};

	void Transform_detachParent(Transform* p_transform)
	{
		if (p_transform->Parent)
		{
			// Because the child referential has changed, we must update the local positions to fit the new referential.
			Transform_setLocalPosition(p_transform, Transform_getWorldPosition(p_transform));
			Transform_setLocalRotation(p_transform, Transform_getWorldRotation(p_transform));
			Transform_setLocalScale(p_transform, Transform_getWorldScale(p_transform));

			p_transform->Parent->Childs.erase(Vector_transformComparator, &p_transform);
		}

		p_transform->Parent = nullptr;
	};

	void Transform_setLocalPosition(Transform* p_transform, _Math::Vector3f& p_localPosition)
	{
		if (!_Math::Vector3f_equals(&p_transform->LocalPosition, &p_localPosition))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalPosition = p_localPosition;
		}
	};

	void Transform_setLocalRotation(Transform* p_transform, _Math::Quaternionf& p_localRotation)
	{
		if (!_Math::Quaternionf_equals(&p_transform->LocalRotation, &p_localRotation))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalRotation = p_localRotation;
		}
	};

	void Transform_setLocalScale(Transform* p_transform, _Math::Vector3f& p_localScale)
	{
		if (!_Math::Vector3f_equals(&p_transform->LocalScale, &p_localScale))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalScale = p_localScale;
		}
	};

	_Math::Matrix4x4f Transform_getLocalToWorldMatrix(Transform* p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return p_transform->LocalToWorldMatrix;
	};

	_Math::Matrix4x4f* Transform_getLocalToWorldMatrix_ref(Transform* p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return &p_transform->LocalToWorldMatrix;
	};

	_Math::Matrix4x4f Transform_getWorldToLocalMatrix(Transform* p_transform)
	{
		_Math::Matrix4x4f l_worldToLocal;
		_Math::Matrixf4x4_inv(Transform_getLocalToWorldMatrix_ref(p_transform), &l_worldToLocal);
		return l_worldToLocal;
	};

	_Math::Matrix4x4f Transform_calculateMatrixToProjectFromTransformToAnother(Transform* p_source, Transform* p_target)
	{
		_Math::Matrix4x4f* l_sourceLocalToWorld = Transform_getLocalToWorldMatrix_ref(p_source);
		_Math::Matrix4x4f l_targetWorldToLocal = Transform_getWorldToLocalMatrix(p_target);
		_Math::Matrix4x4f l_transformationMatrix;
		_Math::Matrixf4x4_mul(l_sourceLocalToWorld, &l_targetWorldToLocal, &l_transformationMatrix);
		return l_transformationMatrix;
	};

	_Math::Vector3f Transform_getWorldPosition(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector3f l_result;
		{
			_Math::Vector4f l_worldPosition4f;
			_Math::Matrixf4x4_extractTranslation(&l_localToWorldMatrix, &l_worldPosition4f);
			_Math::Vector3f_build(&l_worldPosition4f, &l_result);
		}
		return l_result;
	};

	_Math::Quaternionf Transform_getWorldRotation(Transform* p_transform)
	{
		_Math::Quaternionf l_return;
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		{
			_Math::Vector3f l_right, l_up, l_forward;
			_Math::Matrix4x4f_right(&l_localToWorldMatrix, &l_right);
			_Math::Matrix4x4f_up(&l_localToWorldMatrix, &l_up);
			_Math::Matrix4x4f_forward(&l_localToWorldMatrix, &l_forward);
			Quaternion_fromAxis(&l_right, &l_up, &l_forward, &l_return);
		}

		return l_return;
	};

	_Math::Vector3f Transform_getWorldScale(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector3f l_return;
		{
			_Math::Vector4f l_scale4f;
			Matrixf4x4_extractScale(&l_localToWorldMatrix, &l_scale4f);
			l_return = { l_scale4f.x, l_scale4f.y, l_scale4f.z };
		}
		return l_return;
	};

	_Math::Vector3f Transform_getUp(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_upLocal4f;
		_Math::Matrix4x4f_up(&l_localToWorld, &l_upLocal4f);
		_Math::Vector3f l_up3f = *(_Math::Vector3f*)(&l_upLocal4f);
		_Math::Vector3f_normalize(&l_up3f);
		return l_up3f;
	};

	_Math::Vector3f Transform_getForward(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_forwardLocal4f;
		_Math::Matrix4x4f_forward(&l_localToWorld, &l_forwardLocal4f);
		_Math::Vector3f l_forward3f = *(_Math::Vector3f*)(&l_forwardLocal4f);
		_Math::Vector3f_normalize(&l_forward3f);
		return l_forward3f;
	};

	void Transform_markMatricsForRecalculation(Transform* p_transform)
	{
		p_transform->MatricesMustBeRecalculated = true;
		p_transform->UserFlag_HasChanged = true;
		for (size_t i = 0; i < p_transform->Childs.size(); i++)
		{
			Transform_markMatricsForRecalculation(*p_transform->Childs.at(i));
		}
	};

	void transform_updateMatricesIfNecessary(Transform* p_transform)
	{
		if (p_transform->MatricesMustBeRecalculated)
		{
			{
				_Math::Matrif4x4_buildTRS(&p_transform->LocalPosition, &p_transform->LocalRotation, &p_transform->LocalScale, &p_transform->LocalToWorldMatrix);

				if (p_transform->Parent)
				{
					_Math::Matrix4x4f l_localToWorldCopy;
					{
						_Math::Matrix4x4f_copy(&p_transform->LocalToWorldMatrix, &l_localToWorldCopy);
					}

					_Math::Matrix4x4f l_parentLocalToWorld = Transform_getLocalToWorldMatrix(p_transform->Parent);
					_Math::Matrixf4x4_mul(&l_parentLocalToWorld, &l_localToWorldCopy, &p_transform->LocalToWorldMatrix);
				}

			}

			p_transform->MatricesMustBeRecalculated = false;
		}
	};


	void Transform_alloc(Transform* p_transform)
	{
		p_transform->Childs.alloc(0);
		Transform_markMatricsForRecalculation(p_transform);
	};

	void Transform_free(Transform* p_transform)
	{
		p_transform->Childs.free();
	};

}