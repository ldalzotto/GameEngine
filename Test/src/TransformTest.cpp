#include <stdexcept>

#include "Math/Math.h"

#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "ECS_Impl/Components/Transform/TransformComponent.h"

using namespace _MathV2;
using namespace _GameEngine::_ECS;

#define TEST_ASSERT(left, right) \
if(left != right) \
{ \
	throw std::runtime_error(""); \
} \

void test_assert(Vector3<float>& l_left, Vector3<float>& l_right)
{
	if (!VectorM::equals(l_left, l_right))
	{
		throw std::runtime_error("");
	}
}

void test_assert(Vector4<float>& l_left, Vector4<float>& l_right)
{
	if (!VectorM::equals(l_left, l_right))
	{
		throw std::runtime_error("");
	}
}

void test_assert(Quaternion<float>& l_left, Quaternion<float>& l_right)
{
	if (!QuaternionM::equals(l_left, l_right))
	{
		throw std::runtime_error("");
	}
}

namespace _GameEngine::_Test
{
	void Transform_simpleTransformation_test()
	{
		TransformComponent l_transform{};

		Vector3<float> l_localScale = { 1.0f, 1.0f, 1.0f };
		_Math::Transform_setLocalScale(&l_transform.Transform, l_localScale);
		test_assert(_Math::Transform_getWorldScale(&l_transform.Transform), l_localScale);

		Vector3<float> l_localPosition = { 1.0f, 1.0f, 1.0f };
		_Math::Transform_setLocalPosition(&l_transform.Transform, l_localPosition);
		test_assert(_Math::Transform_getWorldPosition(&l_transform.Transform), l_localPosition);

		_Math::Quaternionf l_localRotation{ 0.5f, 0.5f, 0.5f, 0.5f };
		_Math::Transform_setLocalRotation(&l_transform.Transform, l_localRotation);
		_Math::Quaternionf l_worldRot = _Math::Transform_getWorldRotation(&l_transform.Transform);

		test_assert(l_worldRot, _Math::Quaternionf{ 0.5f, 0.5f, 0.5f, 0.5f });
	}

	void Transform_parenting_test()
	{
		{
			TransformComponent** l_root;
			{
				l_root = ComponentT_alloc<TransformComponent>();

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 1.0f, 0.0f, -1.0f };
				l_rootInitInfo.LocalRotation = QuaternionM::fromEulerAngle(Vector3<float>{ 0.0f, 1.0f, 0.0f });
				l_rootInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
				TransformComponent_init(l_root, &l_rootInitInfo);
			}

			TransformComponent** l_child1;
			{
				l_child1 = ComponentT_alloc<TransformComponent>();

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 0.0f, 1.0f, 0.0f };
				{
					Vector3<float> l_localEuler{ 0.0f, 1.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 2.0f, 2.0f, 2.0f };
				TransformComponent_init(l_child1, &l_rootInitInfo);
			}

			_Math::Transform_addChild(&(*l_root)->Transform, &(*l_child1)->Transform);

			TransformComponent** l_child2;
			{
				l_child2 = ComponentT_alloc<TransformComponent>();

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 1.0f, 1.0f, 0.0f };
				{
					Vector3<float> l_localEuler{ 0.0f, 1.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 2.0f, 2.0f, 2.0f };
				TransformComponent_init(l_child2, &l_rootInitInfo);
			}

			_Math::Transform_addChild(&(*l_child1)->Transform, &(*l_child2)->Transform);

			Vector3<float> l_worldPositionChild1ValueTest = { 1.0f, 1.0f, -1.0f };
			test_assert(_Math::Transform_getWorldPosition(&(*l_child1)->Transform), l_worldPositionChild1ValueTest);

			_Math::Quaternionf l_wordRotationValueTest;
			{
				Vector3<float> l_localEuler{ 0.0f, 1.0f, 0.0f };
				_Math::Quaternionf l_rotationDelta;
				{
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rotationDelta);
				}
				_Math::Quaternion_mul(&l_rotationDelta, &l_rotationDelta, &l_wordRotationValueTest);

			}

			test_assert(_Math::Transform_getWorldRotation(&(*l_child1)->Transform), l_wordRotationValueTest);

			Vector3<float> l_worldScaleChild2ValueTest = { 4.0f, 4.0f, 4.0f };
			test_assert(_Math::Transform_getWorldScale(&(*l_child2)->Transform), l_worldScaleChild2ValueTest);

			Component_free((_ECS::Component**)l_root);
			Component_free((_ECS::Component**)l_child1);
			Component_free((_ECS::Component**)l_child2);
		}
	};

	void Transform_referenceSwitch()
	{
		{
			TransformComponent** l_root;
			{
				l_root = ComponentT_alloc<TransformComponent>();

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 1.0f, 0.0f, -1.0f };
				{
					Vector3<float> l_localEuler{ 0.0f, 0.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
				TransformComponent_init(l_root, &l_rootInitInfo);
			}

			TransformComponent** l_child1;
			{
				l_child1 = ComponentT_alloc<TransformComponent>();

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 0.0f, 1.0f, 0.0f };
				{
					Vector3<float> l_localEuler{ M_PI, 0.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 2.0f, 2.0f, 2.0f };
				TransformComponent_init(l_child1, &l_rootInitInfo);
			}

			_Math::Transform_addChild(&(*l_root)->Transform, &(*l_child1)->Transform);

			_Math::Matrix4x4f l_child1ToRootMatrix = _Math::Transform_calculateMatrixToProjectFromTransformToAnother(&(*l_child1)->Transform, &(*l_root)->Transform);
			_Math::Vector4f l_child1RelativePosition = { 0.0f, 1.0f, 0.0f, 0.0f };
			{
				_Math::Vector4f l_child1TransformedPosition;
				_Math::Matrixf4x4_mul(&l_child1ToRootMatrix, &l_child1RelativePosition, &l_child1TransformedPosition);
				// The l_child1 is rotated by PI in the x axis
				_Math::Vector4f l_compared = { 0.0f, -2.0f, 0.0f, 0.0f };
				test_assert(l_child1TransformedPosition, l_compared);
			}

			Component_free((_ECS::Component**)l_root);
			Component_free((_ECS::Component**)l_child1);
		}
	};

}


// TODO -> Tests are in a failure state because the _Math::Quaternion_fromAxis() is not calculated with the proper axis.
/*
	In the _Math::Quaternion_fromAxis() calculation, we have.
		right -> (0,0,1)
		up    -> (1,0,0)
		fwd   -> (0,1,0)
*/
int main()
{
	_GameEngine::_Test::Transform_simpleTransformation_test();
	_GameEngine::_Test::Transform_parenting_test();
	_GameEngine::_Test::Transform_referenceSwitch();

	return 0;
};