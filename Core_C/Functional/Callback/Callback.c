#include "Callback.h"

bool Callback_equals(Callback_PTR p_left, Callback_PTR p_right)
{
	return (p_left->Closure == p_right->Closure) && (p_left->Function == p_right->Function);
};

void Callback_call(Callback_PTR p_callback, void* p_input)
{
	if (p_callback->Function)
	{
		p_callback->Function(p_callback->Closure, p_input);
	}
};