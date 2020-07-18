#pragma once

namespace _Core
{
	struct Callback
	{
		void (*Function)(void* p_closure, void* p_input);
		void* Closure;
	};

	bool Callback_equals(Callback* p_left, Callback* p_right, void* p_null);

	void Callback_call(Callback* p_callback, void* p_input);
}