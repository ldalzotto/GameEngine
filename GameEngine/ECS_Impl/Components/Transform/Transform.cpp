#include "Transform.h"

#include "Math/Vector/VectorTransform.h"
#include "Math/Matrix/MatrixTransform.h"
#include "Math/Quaternion/QuaternionTransform.h"

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
			Transform_setLocalPosition(p_transform, Transform_getWorldPosition(p_transform));
			Transform_setLocalRotation(p_transform, Transform_getWorldRotation(p_transform));
			Transform_setLocalScale(p_transform, Transform_getWorldScale(p_transform));

			p_transform->Parent->Childs.erase(Vector_transformComparator, &p_transform);
		}

		p_transform->Parent = nullptr;
	};

	void Transform_setLocalPosition(Transform* p_transform, _Math::Vector3f& p_localPosition)
	{
		if (!_Math::Vector3f_equals(&p_transform->LocalPosition, &p_localPosition))
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalPosition = p_localPosition;
		}
	};

	void Transform_setLocalRotation(Transform* p_transform, _Math::Quaternionf& p_localRotation)
	{
		if (!_Math::Quaternionf_equals(&p_transform->LocalRotation, &p_localRotation))
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalRotation = p_localRotation;
		}
	};

	void Transform_setLocalScale(Transform* p_transform, _Math::Vector3f& p_localScale)
	{
		if (!_Math::Vector3f_equals(&p_transform->LocalScale, &p_localScale))
		{
			transform_markMatricsForRecalculation(p_transform);
			p_transform->LocalScale = p_localScale;
		}
	};

	_Math::Matrix4x4f Transform_getLocalToWorldMatrix(Transform* p_transform, bool p_includeSelf)
	{
		_Math::Matrix4x4f l_return = _Math::Matrix4x4f_identity();

		Transform* l_currentTransform = nullptr;
		if (p_includeSelf)
		{
			l_currentTransform = p_transform;
		}
		else
		{
			l_currentTransform = p_transform->Parent;
		}

		while (l_currentTransform != nullptr)
		{
			transform_updateMatricesIfNecessary(l_currentTransform);

			_Math::Matrix4x4f l_returnCopy;
			{
				_Math::Matrix4x4f_copy(&l_return, &l_returnCopy);
			}
			_Math::Matrixf4x4_mul(&l_currentTransform->LocalToWorldMatrix, &l_returnCopy, &l_return);
			l_currentTransform = l_currentTransform->Parent;
		}
		return l_return;
	};

	_Math::Vector3f Transform_getWorldPosition(Transform* p_transform)
	{
		_Math::Vector4f l_localPosition4f;
		{
			_Math::Vector4f_build(&p_transform->LocalPosition, 1.0f, &l_localPosition4f);
		}
		_Math::Matrix4x4f l_localToWorldMatrix = Transform_getLocalToWorldMatrix(p_transform, false);
		_Math::Vector3f l_result;
		{
			_Math::Vector4f l_worldPosition4f;
			_Math::Matrixf4x4_mul(&l_localToWorldMatrix, &l_localPosition4f, &l_worldPosition4f);
			_Math::Vector3f_build(&l_worldPosition4f, &l_result);
		}
		return l_result;
	};

	_Math::Quaternionf Transform_getWorldRotation(Transform* p_transform)
	{
		_Math::Quaternionf l_return;
		{
			_Math::Vector3f l_eulerAngles{ 0.0f, 0.0f, 0.0f };
			_Math::Quaternion_fromEulerAngles(&l_eulerAngles, &l_return);
		}

		if (p_transform->Parent)
		{
			_Math::Quaternionf l_parentRotation = Transform_getWorldRotation(p_transform->Parent);
			_Math::Quaternionf l_returnCopy;
			{
				_Math::Quaternionf_copy(&l_return, &l_returnCopy);
			}
			_Math::Quaternion_mul(&l_parentRotation, &l_returnCopy, &l_return);
		}

		_Math::Quaternionf l_returnCopy;
		{
			_Math::Quaternionf_copy(&l_return, &l_returnCopy);
		}
		_Math::Quaternion_mul(&l_returnCopy, &p_transform->LocalRotation, &l_return);
		return l_return;
	};

	_Math::Vector3f Transform_getWorldScale(Transform* p_transform)
	{
		_Math::Vector3f l_return = p_transform->LocalScale;
		if (p_transform->Parent)
		{
			_Math::Vector3f l_parentWorldScale = Transform_getWorldScale(p_transform->Parent);
			_Math::Vector3f_mul(&l_return, &l_parentWorldScale, &l_return);
		}
		return l_return;
	};

	_Math::Vector3f Transform_getUp(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform, false);
		_Math::Vector4f l_upLocal4f;
		l_localToWorld.up(&l_upLocal4f);
		_Math::Vector3f l_up3f = *(_Math::Vector3f*)(&l_upLocal4f);
		_Math::Vector3f_normalize(&l_up3f);
		return l_up3f;
	};

	_Math::Vector3f Transform_getForward(Transform* p_transform)
	{
		_Math::Matrix4x4f l_localToWorld = Transform_getLocalToWorldMatrix(p_transform, false);
		_Math::Vector4f l_forwardLocal4f;
		l_localToWorld.forward(&l_forwardLocal4f);
		_Math::Vector3f l_forward3f = *(_Math::Vector3f*)(&l_forwardLocal4f);
		_Math::Vector3f_normalize(&l_forward3f);
		return l_forward3f;
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
		if (p_transform->MatricesMustBeRecalculated)
		{
			glm::vec3 l_positionGLM, l_scaleGLM;
			glm::quat l_rotationGLM;
			_Math::Vector3f_toGLM(&p_transform->LocalPosition, &l_positionGLM);
			_Math::Quaternionf_toGLM(&p_transform->LocalRotation, &l_rotationGLM);
			_Math::Vector3f_toGLM(&p_transform->LocalScale, &l_scaleGLM);

			glm::mat4x4 l_trs = _Utils::Math_TRS(l_positionGLM, l_rotationGLM, l_scaleGLM);
			
			_Math::Matrif4x4_buildTRS(&p_transform->LocalPosition, &p_transform->LocalRotation, &p_transform->LocalScale, &p_transform->LocalToWorldMatrix);
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