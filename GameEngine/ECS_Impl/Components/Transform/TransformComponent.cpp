#include "TransformComponent.h"

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(ComponentHeader* p_transformComponent)
	{
		TransformComponent* l_transformComponent = (TransformComponent*)p_transformComponent;
		_Math::Transform_free(&l_transformComponent->Transform);
	};

	void TransformComponent_init(TransformComponent* p_transformComponent, TransformInitInfo* p_transformInitInfo)
	{
		p_transformComponent->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
		p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		p_transformComponent->Transform.LocalScale = p_transformInitInfo->LocalScale;
		_Math::Transform_alloc(&p_transformComponent->Transform);

		p_transformComponent->ComponentHeader.OnComponentFree = (OnComponentFunction) TransformComponent_free;
	};

	TransformComponent* TransformComponent_castFromTransform(_Math::Transform* p_transform)
	{
		return (TransformComponent*)(((char*)p_transform) - offsetof(TransformComponent, Transform));
	};
}