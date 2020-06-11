#include "TransformTest.h"

#include <stdexcept>
#include "ECS_Impl/Components/Transform/Transform.h"

using namespace _GameEngine::_ECS;

#define TEST_ASSERT(left, right) \
if(left != right) \
{ \
	throw std::runtime_error(""); \
} \

namespace _GameEngine::_Test
{
	void Transform_simpleTransformation_test()
	{
		Transform l_transform{};
		Transform_setLocalPosition(&l_transform, glm::vec3(1.0f, 1.0f, 1.0f));
		TEST_ASSERT(Transform_getWorldPosition(&l_transform), glm::vec3(1.0f, 1.0f, 1.0f));

		Transform_setLocalRotation(&l_transform, glm::quat(0.5f, 0.5f, 0.5f, 0.5f));
		glm::quat l_worldRot = Transform_getWorldRotation(&l_transform);
		TEST_ASSERT(l_worldRot, glm::quat(0.5f, 0.5f, 0.5f, 0.5f));

		Transform_setLocalScale(&l_transform, glm::vec3(1.0f, 1.0f, 1.0f));
		TEST_ASSERT(Transform_getWorldScale(&l_transform), glm::vec3(1.0f, 1.0f, 1.0f));
	}

	void Transform_parenting_test()
	{
		{
			Component* l_rootComponent;
			Transform* l_root;
			{
				l_rootComponent = Component_alloc(TransformType, sizeof(Transform));
				l_root = (Transform*)l_rootComponent->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = glm::vec3(1.0f, 0.0f, -1.0f);
				l_rootInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f));
				l_rootInitInfo.LocalScale = glm::vec3(1.0f, 1.0f, 1.0f);
				Transform_init(l_rootComponent, &l_rootInitInfo);
			}

			Component* l_child1Component;
			Transform* l_child1;
			{
				l_child1Component = Component_alloc(TransformType, sizeof(Transform));
				l_child1 = (Transform*)l_child1Component->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = glm::vec3(0.0f, 1.0f, 0.0f);
				l_rootInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f));
				l_rootInitInfo.LocalScale = glm::vec3(2.0f, 2.0f, 2.0f);
				Transform_init(l_child1Component, &l_rootInitInfo);
			}

			Transform_addChild(l_root, l_child1);

			Component* l_child2Component;
			Transform* l_child2;
			{
				l_child2Component = Component_alloc(TransformType, sizeof(Transform));
				l_child2 = (Transform*)l_child2Component->Child;

				TransformInitInfo l_rootInitInfo{};
				l_rootInitInfo.LocalPosition = glm::vec3(1.0f, 1.0f, 0.0f);
				l_rootInitInfo.LocalRotation = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f));
				l_rootInitInfo.LocalScale = glm::vec3(2.0f, 2.0f, 2.0f);
				Transform_init(l_child2Component, &l_rootInitInfo);
			}

			Transform_addChild(l_child1, l_child2);

			TEST_ASSERT(Transform_getWorldPosition(l_child1), glm::vec3(1.0f, 1.0f, -1.0f));
			TEST_ASSERT(Transform_getWorldRotation(l_child1), glm::quat(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::quat(glm::vec3(0.0f, 1.0f, 0.0f)));
			TEST_ASSERT(Transform_getWorldScale(l_child2), glm::vec3(4.0f, 4.0f, 4.0f));
			
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