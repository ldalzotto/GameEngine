#include "TransformComponent.h"

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(void* p_transformComponent, void* p_null)
	{
		TransformComponent* l_transformComponent = (TransformComponent*)p_transformComponent;
		_Math::Transform_free(&l_transformComponent->Transform);
	};

	void TransformComponent_init(Component* p_component, TransformInitInfo* p_transformInitInfo)
	{
		TransformComponent* p_transformComponent = (TransformComponent*)(p_component->Child);
		p_transformComponent->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
		p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		p_transformComponent->Transform.LocalScale = p_transformInitInfo->LocalScale;
		_Math::Transform_alloc(&p_transformComponent->Transform);

		p_transformComponent->OnComponentDetached.Closure = p_transformComponent;
		p_transformComponent->OnComponentDetached.Function = TransformComponent_free;
		Core_Observer_register(&p_component->OnComponentFree, &p_transformComponent->OnComponentDetached);
	};

}