#pragma once

typedef struct Callback
{
	void (*Function)(void* p_closure, void* p_input);
	void* Closure;
} Callback;

void Core_Callback_call(Callback* p_callback, void* p_input);