#include "Callback.hpp"

namespace _Core
{
	bool CallbackP_equals(Callback** p_left, Callback** p_right, void* p_null)
	{
		return *p_left == *p_right;
	};

	void Callback_call(Callback* p_callback, void* p_input)
	{
		if (p_callback->Function)
		{
			p_callback->Function(p_callback->Closure, p_input);
		}
	};
}