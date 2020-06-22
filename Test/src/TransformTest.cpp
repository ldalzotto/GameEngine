#include "TransformTest.h"

#include <stdexcept>

#include "Math/Quaternion/QuaternionMath.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"

#include "ECS_Impl/Components/Transform/TransformComponent.h"

using namespace _GameEngine::_ECS;

#define TEST_ASSERT(left, right) \
if(left != right) \
{ \
	throw std::runtime_error(""); \
} \

void test_assert(_GameEngine::_Math::Vector3f& l_left, _GameEngine::_Math::Vector3f& l_right)
{
	if (!_GameEngine::_Math::Vector3f_equals(&l_left, &l_right))
	{
		throw std::runtime_error("");
	}
}

void test_assert(_GameEngine::_Math::Quaternionf& l_left, _GameEngine::_Math::Quaternionf& l_right)
{
	if (!_GameEngine::_Math::Quaternionf_equals(&l_left, &l_right))
	{
		throw std::runtime_error("");
	}
}

namespace _GameEngine::_Test
{
	void Transform_simpleTransformation_test()
	{
		TransformComponent l_transform{};

		_Math::Vector3f l_localScale = { 1.0f, 1.0f, 1.0f };
		_Math::Transform_setLocalScale(&l_transform.Transform, l_localScale);
		test_assert(_Math::Transform_getWorldScale(&l_transform.Transform), l_localScale);

		_Math::Vector3f l_localPosition = { 1.0f, 1.0f, 1.0f };
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
			Component* l_rootComponent;
			TransformComponent* l_root;
			{
				l_rootComponent = Component_alloc(TransformComponentType, sizeof(TransformComponent));
				l_root = (TransformComponent*)l_rootComponent->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 1.0f, 0.0f, -1.0f };
				{
					_Math::Vector3f l_localEuler{ 0.0f, 1.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 1.0f, 1.0f, 1.0f };
				TransformComponent_init(l_rootComponent, &l_rootInitInfo);
			}

			Component* l_child1Component;
			TransformComponent* l_child1;
			{
				l_child1Component = Component_alloc(TransformComponentType, sizeof(TransformComponent));
				l_child1 = (TransformComponent*)l_child1Component->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 0.0f, 1.0f, 0.0f };
				{
					_Math::Vector3f l_localEuler{ 0.0f, 1.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 2.0f, 2.0f, 2.0f };
				TransformComponent_init(l_child1Component, &l_rootInitInfo);
			}

			_Math::Transform_addChild(&l_root->Transform, &l_child1->Transform);

			Component* l_child2Component;
			TransformComponent* l_child2;
			{
				l_child2Component = Component_alloc(TransformComponentType, sizeof(TransformComponent));
				l_child2 = (TransformComponent*)l_child2Component->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = { 1.0f, 1.0f, 0.0f };
				{
					_Math::Vector3f l_localEuler{ 0.0f, 1.0f, 0.0f };
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rootInitInfo.LocalRotation);
				}
				l_rootInitInfo.LocalScale = { 2.0f, 2.0f, 2.0f };
				TransformComponent_init(l_child2Component, &l_rootInitInfo);
			}

			_Math::Transform_addChild(&l_child1->Transform, &l_child2->Transform);

			_Math::Vector3f l_worldPositionChild1ValueTest = { 1.0f, 1.0f, -1.0f };
			test_assert(_Math::Transform_getWorldPosition(&l_child1->Transform), l_worldPositionChild1ValueTest);

			_Math::Quaternionf l_wordRotationValueTest;
			{
				_Math::Vector3f l_localEuler{ 0.0f, 1.0f, 0.0f };
				_Math::Quaternionf l_rotationDelta;
				{
					_Math::Quaternion_fromEulerAngles(&l_localEuler, &l_rotationDelta);
				}
				_Math::Quaternion_mul(&l_rotationDelta, &l_rotationDelta, &l_wordRotationValueTest);
			}

			test_assert(_Math::Transform_getWorldRotation(&l_child1->Transform), l_wordRotationValueTest);

			_Math::Vector3f l_worldScaleChild2ValueTest = { 4.0f, 4.0f, 4.0f };
			 test_assert(_Math::Transform_getWorldScale(&l_child2->Transform), l_worldScaleChild2ValueTest);

			 /*
			{
				// _Math::Matrix4x4f l_localToWorld = _Math::Transform_getLocalToWorldMatrix(&l_child2->Transform);
				_Math::Transform_getLocalToWorldMatrix(&l_child2->Transform);
				_Math::Vector3f l_c0 = { l_child2->Transform.Parent->Parent->LocalToWorldMatrix._00, l_child2->Transform.Parent->Parent->LocalToWorldMatrix._01 , l_child2->Transform.Parent->Parent->LocalToWorldMatrix._02 };
				float l_lenght = _Math::Vector3f_length(&l_c0);
				int ldz = 0;
			}
			*/

			Component_free(&l_rootComponent);
			Component_free(&l_child1Component);
			Component_free(&l_child2Component);
		}
	}

	void TransformTest_test()
	{
		Transform_simpleTransformation_test();
		Transform_parenting_test();
	};
}