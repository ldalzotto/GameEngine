#include "TransformComponent.h"

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(TransformComponent** p_transformComponent)
	{
		TransformComponent* l_transformComponent = *p_transformComponent;
		_Math::Transform_free(&l_transformComponent->Transform);
	};

	void TransformComponent_init(TransformComponent** p_transformComponent, TransformInitInfo* p_transformInitInfo)
	{
		(*p_transformComponent)->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
		(*p_transformComponent)->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		(*p_transformComponent)->Transform.LocalScale = p_transformInitInfo->LocalScale;
		_Math::Transform_alloc(&(*p_transformComponent)->Transform);

		((_ECS::Component*)p_transformComponent)->OnComponentFree = (OnComponentFunction) TransformComponent_free;
	};
}