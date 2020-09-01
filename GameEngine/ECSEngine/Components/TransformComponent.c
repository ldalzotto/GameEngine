#include "TransformComponent.h"


#include "v2/_interface/TransformC.h"
#include "Types_def.h"
#include "ECS.h"


const TransformInitInfo TransformInitInfo_Default =
{
	.LocalPosition = { 0.0f, 0.0f, 0.0f },
	.LocalRotation = { 0.0f, 0.0f, 0.0f, 1.0f },
	.LocalScale = { 1.0f, 1.0f, 1.0f }
};

void TransformComponent_init(TransformComponent* p_transformComponent, TransformInitInfo* p_transformInitInfo)
{
	p_transformComponent->Header.ComponentType = TRANSFORM_COMPONENT_TYPE;
	p_transformComponent->Transform.LocalPosition = p_transformInitInfo->LocalPosition;
	p_transformComponent->Transform.LocalRotation = p_transformInitInfo->LocalRotation;
	p_transformComponent->Transform.LocalScale = p_transformInitInfo->LocalScale;
	Transform_Alloc(&p_transformComponent->Transform);
};

void TransformComponent_Free(TransformComponent* p_transformComponent, ECS* p_ecs)
{
	if (p_transformComponent->Transform.Childs.Size > 0)
	{
		for(size_t i=0;i< p_transformComponent->Transform.Childs.Size;i++)
		{ 
			TransformComponent_PTR l_childComponent = TransformComponent_castFromTransform(p_transformComponent->Transform.Childs.Memory[i]);
			ECS_FreeEntity(p_ecs, l_childComponent->Header.AttachedEntity);
		}
	}

	Transform_Free(&p_transformComponent->Transform);
};

TransformComponent* TransformComponent_castFromTransform(Transform_PTR p_transform)
{
	return (TransformComponent*)(((char*)p_transform) - offsetof(TransformComponent, Transform));
};