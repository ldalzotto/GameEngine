#include "Transform.h"

#include "Utils/Math/Math.h"

namespace _GameEngine::_ECS
{

	ComponentType TransformType = "Transform";

    void transform_updateMatricesIfNecessary(Transform* p_transform);

	void Transform_setLocalPosition(Transform* p_transform, const glm::vec3& p_localPosition)
	{
		if (p_transform->LocalPosition != p_localPosition)
		{
			p_transform->MatricesMustBeRecalculated = true;
			p_transform->LocalPosition = p_localPosition;
		}
	};

	void Transform_setLocalRotation(Transform* p_transform, const glm::quat& p_localRotation)
	{
		if (p_transform->LocalRotation != p_localRotation)
		{
			p_transform->MatricesMustBeRecalculated = true;
			p_transform->LocalRotation = p_localRotation;
		}
	};

	void Transform_setLocalScale(Transform* p_transform, const glm::vec3& p_localScale) 
	{
		if (p_transform->LocalScale != p_localScale)
		{
			p_transform->MatricesMustBeRecalculated = true;
			p_transform->LocalScale = p_localScale;
		}
	};

	glm::mat4x4* Transform_getLocalToWorldMatrix(Transform* p_transform)
	{
		transform_updateMatricesIfNecessary(p_transform);
		return &p_transform->LocalToWorldMatrix;
	};

	glm::vec3 Transform_getWorldPosition(Transform* p_transform)
	{
		return *Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(p_transform->LocalPosition, 0.0f);
	};

	glm::vec3 Transform_getUp(Transform* p_transform)
	{
		return glm::normalize(*Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	};

	glm::vec3 Transform_getForward(Transform* p_transform)
	{
		return glm::normalize(*Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	};

	void transform_updateMatricesIfNecessary(Transform* p_transform)
	{
		if(p_transform->MatricesMustBeRecalculated)
		{
			p_transform->LocalToWorldMatrix = _Utils::Math_TRS(p_transform->LocalPosition, p_transform->LocalRotation, p_transform->LocalScale);
			p_transform->MatricesMustBeRecalculated = false;
		}
	};

	void Transform_init(Transform* p_transform, TransformInitInfo* p_transformInitInfo)
	{
		p_transform->LocalPosition = p_transformInitInfo->LocalPosition;
		p_transform->LocalRotation = p_transformInitInfo->LocalRotation;
		p_transform->LocalScale = p_transformInitInfo->LocalScale;
		p_transform->MatricesMustBeRecalculated = true;
	};


}