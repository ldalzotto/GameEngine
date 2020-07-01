#pragma once

namespace _GameEngine::_ECS
{
	struct Component;
}

#define COMPONENT_ALLOC(ComponentTypeName, OutComponentPtrName) \
	_GameEngine::_ECS::Component* ##OutComponentPtrName = _GameEngine::_ECS::Component_alloc(##ComponentTypeName##Type, sizeof(##ComponentTypeName));

#define COMPONENT_GET_CHILD(ComponentTypeName, ComponentPtrName, OutComponentChildPtrName) \
	ComponentTypeName* OutComponentChildPtrName; \
	{ \
		##OutComponentChildPtrName = (##ComponentTypeName*)##ComponentPtrName->Child; \
	}