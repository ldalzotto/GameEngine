#include "Callback.hpp"

namespace _Core
{
	bool Callback_equals(Callback* p_left, Callback* p_right, void* p_null)
	{
		return (p_left->Closure == p_right->Closure) && (p_left->Function == p_right->Function);
	};

	void Callback_call(Callback* p_callback, void* p_input)
	{
		if (p_callback->Function)
		{
			p_callback->Function(p_callback->Closure, p_input);
		}
	};
}