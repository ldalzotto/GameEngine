#include "Transform.h"

#include "Utils/Math/Math.h"

namespace _GameEngine::_ECS
{
	bool Vector_transformComparator(Transform** left, Transform** right)
	{
		return *left == *right;
	}
}

namespace _GameEngine::_ECS
{
	ComponentType TransformType = "Transform";

	void transform_markMatricsForRecalculation(Transform* p_transform);
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

			transform_markMatricsForRecalculation(p_newChild);
		}
	};

	void Transform_detachParent(Transform* p_transform)
	{
		if (p_transform->Parent)
		{
			// Because the child referential has changed, we must update the local positions to fit the new referential.
			p_transform->LocalPosition = Transform_getWorldPosition(p_transform);

			p_transform->Parent->Childs.erase(Vector_transformComparator, &p_transform);
		}

		p_transform->Parent = nullptr;
	};

	void Transform_setLocalPosition(Transform* p_transform, const glm::vec3& p_localPosition)
	{
		if (p_transform->LocalPosition != p_localPosition)
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalPosition = p_localPosition;
		}
	};

	void Transform_setLocalRotation(Transform* p_transform, const glm::quat& p_localRotation)
	{
		if (p_transform->LocalRotation != p_localRotation)
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalRotation = p_localRotation;
		}
	};

	void Transform_setLocalScale(Transform* p_transform, const glm::vec3& p_localScale) 
	{
		if (p_transform->LocalScale != p_localScale)
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalScale = p_localScale;
		}
	};

	glm::mat4x4 Transform_getLocalToWorldMatrix(Transform* p_transform)
	{
		glm::mat4x4 l_return = glm::mat4x4{};
		l_return[0][0] = 1.0f;
		l_return[1][1] = 1.0f;
		l_return[2][2] = 1.0f;
		l_return[3][3] = 1.0f;

		if (p_transform->Parent)
		{
			l_return = l_return * Transform_getLocalToWorldMatrix(p_transform->Parent);
		}

		transform_updateMatricesIfNecessary(p_transform);
		return l_return * p_transform->LocalToWorldMatrix;
	};

	glm::vec3 Transform_getWorldPosition(Transform* p_transform)
	{
		return Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(p_transform->LocalPosition, 0.0f);
	};

	glm::vec3 Transform_getUp(Transform* p_transform)
	{
		return glm::normalize(Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	};

	glm::vec3 Transform_getForward(Transform* p_transform)
	{
		return glm::normalize(Transform_getLocalToWorldMatrix(p_transform) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	};


	void transform_markMatricsForRecalculation(Transform* p_transform)
	{
		p_transform->MatricesMustBeRecalculated = true;
		p_transform->HasChangedThisFrame = true;
		for (size_t i = 0; i < p_transform->Childs.size(); i++)
		{
			transform_markMatricsForRecalculation(*p_transform->Childs.at(i));
		}
	};

	void transform_updateMatricesIfNecessary(Transform* p_transform)
	{
		if(p_transform->MatricesMustBeRecalculated)
		{
			p_transform->LocalToWorldMatrix = _Utils::Math_TRS(p_transform->LocalPosition, p_transform->LocalRotation, p_transform->LocalScale);
			p_transform->MatricesMustBeRecalculated = false;
		}
	};

	void Transform_free(void* p_transform, void* p_null)
    {
        Transform* l_transform = (Transform*)p_transform;
        l_transform->Childs.free();
    };

	void Transform_init(Component* p_transform, TransformInitInfo* p_transformInitInfo)
	{
        Transform* l_transform = (Transform*)p_transform->Child;
	    l_transform->Childs.alloc(0);
		l_transform->LocalPosition = p_transformInitInfo->LocalPosition;
		l_transform->LocalRotation = p_transformInitInfo->LocalRotation;
		l_transform->LocalScale = p_transformInitInfo->LocalScale;
		transform_markMatricsForRecalculation(l_transform);

        l_transform->OnComponentDetached.Closure = l_transform;
		l_transform->OnComponentDetached.Callback = Transform_free;
		_Utils::Observer_register(&p_transform->ComponentFreeEvent, &l_transform->OnComponentDetached);
	};


}