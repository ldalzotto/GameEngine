#pragma once

namespace _GameEngine::_Utils
{
	template<class T>
	struct OptionalT
	{
		bool HasValue;
		T Value;

		T* getValue() { if (HasValue) { return &Value; } else { return nullptr; } };
	};
}