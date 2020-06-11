#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "DataStructures/VectorT.h"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformType;

	struct Transform
	{
		Transform* Parent;
	    _Core::VectorT<Transform*> Childs;

		bool MatricesMustBeRecalculated;
		bool HasChangedThisFrame;
		glm::vec3 LocalPosition;
		glm::quat LocalRotation;
		glm::vec3 LocalScale;
		glm::mat4x4 LocalToWorldMatrix;

        _Utils::Subject OnComponentDetached;
    };

	void Transform_addChild(Transform* p_transform, Transform* p_newChild);
	void Transform_detachParent(Transform* p_transform);

	void Transform_setLocalPosition(Transform* p_transform, const glm::vec3& p_localPosition);
	void Transform_setLocalRotation(Transform* p_transform, const glm::quat& p_localRotation);
	void Transform_setLocalScale(Transform* p_transform, const glm::vec3& p_localScale);
	glm::mat4x4 Transform_getLocalToWorldMatrix(Transform* p_transform, bool p_includeSelf = true);

	glm::vec3 Transform_getWorldPosition(Transform* p_transform);
	glm::quat Transform_getWorldRotation(Transform* p_transform);
	glm::vec3 Transform_getWorldScale(Transform* p_transform);

	glm::vec3 Transform_getUp(Transform* p_transform);
	glm::vec3 Transform_getForward(Transform* p_transform);

	struct TransformInitInfo
	{
		glm::vec3 LocalPosition;
		glm::quat LocalRotation;
		glm::vec3 LocalScale;
	};

	void Transform_init(Component* p_transform, TransformInitInfo* p_transformInitInfo);
}