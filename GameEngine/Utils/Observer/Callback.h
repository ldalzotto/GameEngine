#pragma once

namespace _GameEngine::_Utils
{
	struct Callback
	{
		void(*Function)(void* p_closure, void* p_inputParameter);
		void* Closure;
	};

	void Callback_invoke(Callback* p_callback, void* p_inputParameter);
}