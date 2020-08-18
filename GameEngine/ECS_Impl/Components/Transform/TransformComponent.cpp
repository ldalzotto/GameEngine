#include "TransformComponent.h"

#include "ECS/ECS.h"
#include "ECS/ECSEventQueueT.hpp"
extern "C"
{
#include "v2/_interface/TransformC.h"
}

namespace _GameEngine::_ECS
{
	ComponentType TransformComponentType = "TransformComponent";

	void TransformComponent_free(TransformComponent* p_transformComponent, ECS* p_ecs)
	{
		if (p_transformComponent->Transform.Childs.Size > 0)
		{
			ARRAY_TRANSFORMHANDLE_ITERATOR l_childs; Arr_BuildIterator_TransformPtr(&p_transformComponent->Transform.Childs, &l_childs);
			while (Iter_MoveNext_TransformPtr(&l_childs))
			{
				_ECS::TransformComponent* l_childComponent = TransformComponent_castFromTransform((*l_childs.Current));
				_ECS::ECSEventMessage* l_removeMessage = _ECS::ECSEventMessage_removeEntity_alloc(&l_childComponent->ComponentHeader.AttachedEntity);
				_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_removeMessage);
			}
		}

		Transform_Free(&p_transformComponent->Transform);
	};

	void TransformComponent_init(TransformComponent* p_transformComponent, TransformInitInfo* p_transformInitInfo)
	{
		p_transformComponent->Transform.LocalPosition = *(VECTOR3F_PTR)&p_transformInitInfo->LocalPosition;
		p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
		p_transformComponent->Transform.LocalScale = *(VECTOR3F_PTR)&p_transformInitInfo->LocalScale;
		Transform_Alloc(&p_transformComponent->Transform);

		p_transformComponent->ComponentHeader.OnComponentFree = TransformComponent_free;
	};

	TransformComponent* TransformComponent_castFromTransform(TRANSFORM_PTR p_transform)
	{
		return (TransformComponent*)(((char*)p_transform) - offsetof(TransformComponent, Transform));
	};
}