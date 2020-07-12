#include "Callback.h"

void Core_Callback_call(Callback* p_callback, void* p_input)
{
	if (p_callback->Function)
	{
		p_callback->Function(p_callback->Closure, p_input);
	}
}