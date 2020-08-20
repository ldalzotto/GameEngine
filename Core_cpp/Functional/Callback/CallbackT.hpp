#pragma once

extern "C"
{
	#include "Functional/Callback/Callback.h"
}

namespace _Core
{
	template <typename CLOSURE_TYPE, typename INPUT_TYPE>
	struct CallbackT
	{
		void (*Function)(CLOSURE_TYPE* p_closure, INPUT_TYPE* p_input);
		CLOSURE_TYPE* Closure;
	};

	template <typename CLOSURE_TYPE, typename INPUT_TYPE>
	inline void CallbackT_call(CallbackT<CLOSURE_TYPE, INPUT_TYPE>* p_callback, INPUT_TYPE* p_input)
	{
		Callback_call((Callback*)p_callback, p_input);
	};
}