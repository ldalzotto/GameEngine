#include "TransformM.hpp"

#include "v2/Vector/RVector.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"

namespace _MathV2
{
	bool Vector_transformComparator(Transform** left, Transform** right, void*)
	{
		return *left == *right;
	}
}

namespace _MathV2
{
	void transform_updateMatricesIfNecessary(Transform& p_transform);

	void TransformM::addChild(TransformHandle p_transform, TransformHandle p_newChild)
	{
		if (p_newChild->Parent != p_transform)
		{
			if (p_newChild->Parent)
			{
				_Core::VectorT_eraseCompare(&p_newChild->Parent->Childs, _Core::ComparatorT<Transform*, Transform*, void>{ Vector_transformComparator, &p_newChild });
			}

			p_newChild->Parent = p_transform;
			_Core::VectorT_pushBack(&p_transform->Childs, &p_newChild);

			TransformM::markMatricsForRecalculation(*p_newChild);
		}
	};

	void TransformM::detachParent(TransformHandle p_transform)
	{
		if (p_transform->Parent)
		{
			// Because the child referential has changed, we must update the local positions to fit the new referential.
			TransformM::setLocalPosition(*p_transform, TransformM::getWorldPosition(*p_transform));
			TransformM::setLocalRotation(*p_transform, TransformM::getWorldRotation(*p_transform));
			TransformM::setLocalScale(*p_transform, TransformM::getWorldScale(*p_transform));

			_Core::VectorT_eraseCompare(&p_transform->Parent->Childs, _Core::ComparatorT<Transform*, Transform*, void>{ Vector_transformComparator, &p_transform });
		}

		p_transform->Parent = nullptr;
	};

	void TransformM::setLocalPosition(Transform& p_transform, Vector3<float>& p_localPosition)
	{
		if (!VectorM::equals(p_transform.LocalPosition, p_localPosition))
		{
			TransformM::markMatricsForRecalculation(p_transform);
			p_transform.LocalPosition = p_localPosition;
		}
	};

	void TransformM::setLocalRotation(Transform& p_transform, const Quaternion<float>& p_localRotation)
	{
		if (!QuaternionM::equals(p_transform.LocalRotation, p_localRotation))
		{
			TransformM::markMatricsForRecalculation(p_transform);
			p_transform.LocalRotation = p_localRotation;
		}
	};

	void TransformM::setLocalScale(Transform& p_transform, Vector3<float>& p_localScale)
	{
		if (!VectorM::equals(p_transform.LocalScale, p_localScale))
		{
			TransformM::markMatricsForRecalculation(p_transform);
			p_transform.LocalScale = p_localScale;
		}
	};

	void TransformM::setWorldPosition(Transform& p_transform, Vector3<float>& p_worldPosition)
	{
		if (p_transform.Parent == nullptr)
		{
			TransformM::setLocalPosition(p_transform, p_worldPosition);
		}
		else
		{
			Vector4<float> l_localPosition = MatrixM::mul(
				TransformM::getWorldToLocalMatrix(*p_transform.Parent),
				VectorM::cast(p_worldPosition, 1.0f)
			);
			TransformM::setLocalPosition(p_transform, VectorM::cast(l_localPosition));
		}
	};

	void TransformM::addToWorldPosition(Transform& p_transform, Vector3<float>& p_worldPosition_delta)
	{
		TransformM::setWorldPosition(p_transform, VectorM::add(TransformM::getWorldPosition(p_transform), p_worldPosition_delta));
	};

	Matrix4x4<float> TransformM::getLocalToWorldMatrix(Transform& p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return p_transform.LocalToWorldMatrix;
	};

	Matrix4x4<float> TransformM::getWorldToLocalMatrix(Transform& p_transform)
	{
		return MatrixM::inv(TransformM::getLocalToWorldMatrix(p_transform));
	};

	Matrix4x4<float> TransformM::calculateMatrixToProjectFromTransformToAnother(Transform& p_source, Transform& p_target)
	{
		return MatrixM::mul(TransformM::getLocalToWorldMatrix(p_source), TransformM::getWorldToLocalMatrix(p_target));
	};

	Vector3<float> TransformM::getWorldPosition(Transform& p_transform)
	{
		return VectorM::cast(MatrixM::getTranslation(TransformM::getLocalToWorldMatrix(p_transform)));
	};

	Quaternion<float> TransformM::getWorldRotation(Transform& p_transform)
	{
		Matrix4x4<float> l_localToWorldMatrix = TransformM::getLocalToWorldMatrix(p_transform);

		return QuaternionM::fromAxis(
			MatrixM::build(
				VectorM::normalize(VectorM::cast(MatrixM::right(l_localToWorldMatrix))),
				VectorM::normalize(VectorM::cast(MatrixM::up(l_localToWorldMatrix))),
				VectorM::normalize(VectorM::cast(MatrixM::forward(l_localToWorldMatrix)))
			)
		);
	};

	Vector3<float> TransformM::getWorldScale(Transform& p_transform)
	{
		return VectorM::cast(MatrixM::getScale(TransformM::getLocalToWorldMatrix(p_transform)));
	};

	Vector3<float> TransformM::getRight(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::right(TransformM::getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> TransformM::getUp(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::up(TransformM::getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> TransformM::getForward(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::forward(TransformM::getLocalToWorldMatrix(p_transform))));
	};

	Vector3<float> TransformM::getRight_worldSpace(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::right(TransformM::getWorldToLocalMatrix(p_transform))));
	};

	Vector3<float> TransformM::getUp_worldSpace(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::up(TransformM::getWorldToLocalMatrix(p_transform))));
	};

	Vector3<float> TransformM::getForward_worldSpace(Transform& p_transform)
	{
		return VectorM::normalize(VectorM::cast(MatrixM::forward(TransformM::getWorldToLocalMatrix(p_transform))));
	};

	void TransformM::markMatricsForRecalculation(Transform& p_transform)
	{
		p_transform.MatricesMustBeRecalculated = true;
		p_transform.UserFlag_HasChanged = true;
		for (size_t i = 0; i < p_transform.Childs.Size; i++)
		{
			TransformM::markMatricsForRecalculation(**_Core::VectorT_at(&p_transform.Childs, i));
		}
	};

	void transform_updateMatricesIfNecessary(Transform& p_transform)
	{
		if (p_transform.MatricesMustBeRecalculated)
		{
			{
				p_transform.LocalToWorldMatrix =
					MatrixM::buildTRS(
						p_transform.LocalPosition,
						p_transform.LocalRotation,
						p_transform.LocalScale
					);

				if (p_transform.Parent)
				{
					p_transform.LocalToWorldMatrix = MatrixM::mul(TransformM::getLocalToWorldMatrix(*p_transform.Parent), p_transform.LocalToWorldMatrix);
				}
			}

			p_transform.MatricesMustBeRecalculated = false;
		}
	};


	void TransformM::alloc(TransformHandle p_transform)
	{
		_Core::VectorT_alloc(&p_transform->Childs, 0);
		TransformM::markMatricsForRecalculation(*p_transform);
	};

	void TransformM::free(TransformHandle p_transform)
	{
		_Core::VectorT_free(&p_transform->Childs);
	};

}