#pragma once 

#include <string>

typedef std::string ComponentType;

namespace _GameEngine::_ECS
{
	template<typename COMPONENT_TYPE>
	extern inline ComponentType* extractComponentType();
}
