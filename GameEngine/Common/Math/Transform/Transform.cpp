#include "Transform.h"

#include "v2/Vector/RVector.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
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

	void Transform_setLocalRotation(Transform* p_transform, const Quaternion<float>& p_localRotation)
	{
		if (!QuaternionM::equals(p_transform->LocalRotation, p_localRotation))
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
			Vector4<float> l_localPosition = MatrixM::mul(
				_Math::Transform_getWorldToLocalMatrix(p_transform->Parent),
				VectorM::cast(p_worldPosition, 1.0f)
			);
			_Math::Transform_setLocalPosition(p_transform, VectorM::cast(l_localPosition));
		}
	};

	void Transform_addToWorldPosition(Transform* p_transform, Vector3<float>& p_worldPosition_delta)
	{
		_Math::Transform_setWorldPosition(p_transform, VectorM::add(_Math::Transform_getWorldPosition(p_transform), p_worldPosition_delta));
	};

	_MathV2::Matrix4x4<float> Transform_getLocalToWorldMatrix(Transform* p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return p_transform->LocalToWorldMatrix;
	};

	_MathV2::Matrix4x4<float>* Transform_getLocalToWorldMatrix_ref(Transform* p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return &p_transform->LocalToWorldMatrix;
	};

	_MathV2::Matrix4x4<float> Transform_getWorldToLocalMatrix(Transform* p_transform)
	{
		return _MathV2::MatrixM::inv(Transform_getLocalToWorldMatrix(p_transform));
	};

	_MathV2::Matrix4x4<float> Transform_calculateMatrixToProjectFromTransformToAnother(Transform* p_source, Transform* p_target)
	{
		return _MathV2::MatrixM::mul(Transform_getLocalToWorldMatrix(p_source), Transform_getWorldToLocalMatrix(p_target));
	};

	Vector3<float> Transform_getWorldPosition(Transform* p_transform)
	{
		return VectorM::cast(MatrixM::getTranslation(Transform_getLocalToWorldMatrix(p_transform)));
	};

	Quaternion<float> Transform_getWorldRotation(Transform* p_transform)
	{
		Matrix4x4<float> l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform);
		
		return QuaternionM::fromAxis(
			MatrixM::build(
				VectorM::normalize(VectorM::cast(MatrixM::right(l_localToWorldMatrix))),
				VectorM::normalize(VectorM::cast(MatrixM::up(l_localToWorldMatrix))),
				VectorM::normalize(VectorM::cast(MatrixM::forward(l_localToWorldMatrix)))
			)
		);
	};

	Vector3<float> Transform_getWorldScale(Transform* p_transform)
	{
		return VectorM::cast(MatrixM::getScale(Transform_getLocalToWorldMatrix(p_transform)));
	};

	Vector3<float> Transform_getRight(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::right(Transform_getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> Transform_getUp(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::up(Transform_getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> Transform_getForward(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::forward(Transform_getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> Transform_getRight_worldSpace(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::right(Transform_getWorldToLocalMatrix(p_transform))));
	};

	Vector3<float> Transform_getUp_worldSpace(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::up(Transform_getWorldToLocalMatrix(p_transform))));
	};

	Vector3<float> Transform_getForward_worldSpace(Transform* p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::forward(Transform_getWorldToLocalMatrix(p_transform))));
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
				p_transform->LocalToWorldMatrix =
					MatrixM::buildTRS(
						p_transform->LocalPosition,
						p_transform->LocalRotation,
						p_transform->LocalScale
					);

				if (p_transform->Parent)
				{
					p_transform->LocalToWorldMatrix = MatrixM::mul(Transform_getLocalToWorldMatrix(p_transform->Parent), p_transform->LocalToWorldMatrix);
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