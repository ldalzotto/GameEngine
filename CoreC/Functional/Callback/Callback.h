#pragma once

#include "DataStructures/GenericArray.h"
#include "DataStructures/GenericArrayNameMacros.h"

typedef struct Callback
{
	void (*Function)(void* p_closure, void* p_input);
	void* Closure;
} Callback;

typedef Core_GenericArray CORE_VECTOR_NAME(Callback);

void Core_Callback_call(Callback* p_callback, void* p_input);