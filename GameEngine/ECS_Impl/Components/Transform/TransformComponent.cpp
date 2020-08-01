#include "TransformComponent.h"

#include "ECS/ECS.h"
#include "ECS/ECSEventQueueT.hpp"
#include "v2/Transform/TransformM.hpp"

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(TransformComponent* p_transformComponent, ECS* p_ecs)
	{
		if (p_transformComponent->Transform.Childs.Size > 0)
		{
			_Core::VectorIteratorT<_MathV2::Transform*> l_childs = _Core::VectorT_buildIterator(&p_transformComponent->Transform.Childs);
			while (_Core::VectorIteratorT_moveNext(&l_childs))
			{
				_ECS::TransformComponent* l_childComponent = TransformComponent_castFromTransform((*l_childs.Current));
				_ECS::ECSEventMessage* l_removeMessage =  _ECS::ECSEventMessage_removeEntity_alloc(&l_childComponent->ComponentHeader.AttachedEntity);
				_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_removeMessage);
			}
		}

		_MathV2::TransformM::free(&p_transformComponent->Transform);
	};

	void TransformComponent_init(TransformComponent* p_transformComponent, TransformInitInfo* p_transformInitInfo)
	{
		p_transformComponent->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
		p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		p_transformComponent->Transform.LocalScale = p_transformInitInfo->LocalScale;
		_MathV2::TransformM::alloc(&p_transformComponent->Transform);

		p_transformComponent->ComponentHeader.OnComponentFree = TransformComponent_free;
	};

	TransformComponent* TransformComponent_castFromTransform(_MathV2::Transform* p_transform)
	{
		return (TransformComponent*)(((char*)p_transform) - offsetof(TransformComponent, Transform));
	};
}