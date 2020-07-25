#include "TransformComponent.h"

#include "ECS/ECS.h"
#include "ECS/ECSEventQueueT.hpp"

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(TransformComponent* p_transformComponent, ECS* p_ecs)
	{
		if (p_transformComponent->Transform.Childs.Size > 0)
		{
			_Core::VectorIteratorT<_Math::Transform*> l_childs = _Core::VectorT_buildIterator(&p_transformComponent->Transform.Childs);
			while (_Core::VectorIteratorT_moveNext(&l_childs))
			{
				_ECS::TransformComponent* l_childComponent = TransformComponent_castFromTransform((*l_childs.Current));
				_ECS::ECSEventMessage* l_removeMessage =  _ECS::ECSEventMessage_removeEntity_alloc(&l_childComponent->ComponentHeader.AttachedEntity);
				_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_removeMessage);
			}
		}

		_Math::Transform_free(&p_transformComponent->Transform);
	};

	void TransformComponent_init(TransformComponent* p_transformComponent, TransformInitInfo* p_transformInitInfo)
	{
		p_transformComponent->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
		p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		p_transformComponent->Transform.LocalScale = p_transformInitInfo->LocalScale;
		_Math::Transform_alloc(&p_transformComponent->Transform);

		p_transformComponent->ComponentHeader.OnComponentFree = TransformComponent_free;
	};

	TransformComponent* TransformComponent_castFromTransform(_Math::Transform* p_transform)
	{
		return (TransformComponent*)(((char*)p_transform) - offsetof(TransformComponent, Transform));
	};
}