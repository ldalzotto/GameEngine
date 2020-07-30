#include "Transform.h"

#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/Vector.h"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"

namespace _GameEngine::_Math
{
	bool Vector_transformComparator(Transform** left, Transform** right, void*)
	{
		return *left == *right;
	}
}

using namespace _MathV2;


namespace _GameEngine::_Math
{
	void transform_updateMatricesIfNecessary(Transform* p_transform);

	void Transform_addChild(Transform* p_transform, Transform* p_newChild)
	{
		if (p_newChild->Parent != p_transform)
		{
			if (p_newChild->Parent)
			{
				_Core::VectorT_eraseCompare(&p_newChild->Parent->Childs, _Core::ComparatorT<Transform*, Transform*, void>{ Vector_transformComparator, &p_newChild });
			}

			p_newChild->Parent = p_transform;
			_Core::VectorT_pushBack(&p_transform->Childs, &p_newChild);

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

			_Core::VectorT_eraseCompare(&p_transform->Parent->Childs, _Core::ComparatorT<Transform*, Transform*, void>{ Vector_transformComparator, &p_transform });
		}

		p_transform->Parent = nullptr;
	};

	void Transform_setLocalPosition(Transform* p_transform, Vector3<float>& p_localPosition)
	{
		if (!VectorM::equals(p_transform->LocalPosition, p_localPosition))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalPosition = p_localPosition;
		}
	};

	void Transform_setLocalRotation(Transform* p_transform, Quaternion<float>& p_localRotation)
	{
		if (!Quaternion_Equals(p_transform->LocalRotation, p_localRotation))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalRotation = p_localRotation;
		}
	};

	void Transform_setLocalScale(Transform* p_transform, Vector3<float>& p_localScale)
	{
		if (!VectorM::equals(p_transform->LocalScale, p_localScale))
		{
			Transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalScale = p_localScale;
		}
	};

	void Transform_setWorldPosition(Transform* p_transform, Vector3<float>& p_worldPosition)
	{
		if (p_transform->Parent == nullptr)
		{
			_Math::Transform_setLocalPosition(p_transform, p_worldPosition);
		}
		else
		{
			_Math::Matrix4x4f l_parentWorldToLocal = _Math::Transform_getWorldToLocalMatrix(p_transform->Parent);
			Vector3<float> l_localPosition;
			_Math::Matrixf4x4_mul(&l_parentWorldToLocal, (_Math::Vector3f*) & p_worldPosition, (_Math::Vector3f*) & l_localPosition);
			_Math::Transform_setLocalPosition(p_transform, l_localPosition);
		}
	};

	void Transform_addToWorldPosition(Transform* p_transform, Vector3<float>& p_worldPosition_delta)
	{
		_Math::Transform_setWorldPosition(p_transform, VectorM::add(_Math::Transform_getWorldPosition(p_transform), p_worldPosition_delta));
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

	Vector3<float> Transform_getWorldPosition(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_worldPosition4f;
		_Math::Matrixf4x4_extractTranslation(&l_localToWorldMatrix, &l_worldPosition4f);

		return *(Vector3<float>*)&l_worldPosition4f;
	};

	Quaternion<float> Transform_getWorldRotation(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		Vector3<float> l_right, l_up, l_forward;
		
		_Math::Matrix4x4f_right(&l_localToWorldMatrix, (_Math::Vector3f*)&l_right);
		_Math::Matrix4x4f_up(&l_localToWorldMatrix, (_Math::Vector3f*) &l_up);
		_Math::Matrix4x4f_forward(&l_localToWorldMatrix, (_Math::Vector3f*) &l_forward);
		
		return QuaternionM::fromAxis(Matrix3x3<float>::build(VectorM::normalize(l_right), VectorM::normalize(l_up), VectorM::normalize(l_forward)));
	};

	Vector3<float> Transform_getWorldScale(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		Vector3<float> l_return;
		{
			_Math::Vector4f l_scale4f;
			Matrixf4x4_extractScale(&l_localToWorldMatrix, &l_scale4f);
			l_return = { l_scale4f.x, l_scale4f.y, l_scale4f.z };
		}
		return l_return;
	};

	Vector3<float> Transform_getRight(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_rightLocal4f;
		_Math::Matrix4x4f_right(&l_localToWorld, &l_rightLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_rightLocal4f));
	};

	Vector3<float> Transform_getUp(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_upLocal4f;
		_Math::Matrix4x4f_up(&l_localToWorld, &l_upLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_upLocal4f));
	};

	Vector3<float> Transform_getForward(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform);
		_Math::Vector4f l_forwardLocal4f;
		_Math::Matrix4x4f_forward(&l_localToWorld, &l_forwardLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_forwardLocal4f));
	};

	Vector3<float> Transform_getRight_worldSpace(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getWorldToLocalMatrix(p_transform);
		_Math::Vector4f l_rightLocal4f;
		_Math::Matrix4x4f_right(&l_localToWorld, &l_rightLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_rightLocal4f));
	};

	Vector3<float> Transform_getUp_worldSpace(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getWorldToLocalMatrix(p_transform);
		_Math::Vector4f l_upLocal4f;
		_Math::Matrix4x4f_up(&l_localToWorld, &l_upLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_upLocal4f));
	};

	Vector3<float> Transform_getForward_worldSpace(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getWorldToLocalMatrix(p_transform);
		_Math::Vector4f l_forwardLocal4f;
		_Math::Matrix4x4f_forward(&l_localToWorld, &l_forwardLocal4f);
		return VectorM::normalize(*(Vector3<float>*)(&l_forwardLocal4f));
	};

	void Transform_markMatricsForRecalculation(Transform* p_transform)
	{
		p_transform->MatricesMustBeRecalculated = true;
		p_transform->UserFlag_HasChanged = true;
		for (size_t i = 0; i < p_transform->Childs.Size; i++)
		{
			Transform_markMatricsForRecalculation(*_Core::VectorT_at(&p_transform->Childs, i));
		}
	};

	void transform_updateMatricesIfNecessary(Transform* p_transform)
	{
		if (p_transform->MatricesMustBeRecalculated)
		{
			{
				_Math::Matrif4x4_buildTRS((_Math::Vector3f*)&p_transform->LocalPosition, (_Math::Quaternionf*) & p_transform->LocalRotation, (_Math::Vector3f*)&p_transform->LocalScale, &p_transform->LocalToWorldMatrix);

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
		_Core::VectorT_alloc(&p_transform->Childs, 0);
		Transform_markMatricsForRecalculation(p_transform);
	};

	void Transform_free(Transform* p_transform)
	{
		_Core::VectorT_free(&p_transform->Childs);
	};

}