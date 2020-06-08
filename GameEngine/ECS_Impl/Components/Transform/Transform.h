#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	extern ComponentType TransformType;

	struct Transform
	{
		bool MatricesMustBeRecalculated;
		glm::vec3 LocalPosition;
		glm::quat LocalRotation;
		glm::vec3 LocalScale;
		glm::mat4x4 LocalToWorldMatrix;
	};

	void Transform_setLocalPosition(Transform* p_transform, const glm::vec3& p_localPosition);
	void Transform_setLocalRotation(Transform* p_transform, const glm::quat& p_localRotation);
	void Transform_setLocalScale(Transform* p_transform, const glm::vec3& p_localScale);
	glm::mat4x4* Transform_getLocalToWorldMatrix(Transform* p_transform);

	glm::vec3 Transform_getWorldPosition(Transform* p_transform);
	glm::vec3 Transform_getUp(Transform* p_transform);
	glm::vec3 Transform_getForward(Transform* p_transform);

	struct TransformInitInfo
	{
		glm::vec3 LocalPosition;
		glm::quat LocalRotation;
		glm::vec3 LocalScale;
	};

	void Transform_init(Transform* p_transform, TransformInitInfo* p_transformInitInfo);
}