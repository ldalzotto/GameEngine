#pragma once

namespace _Core
{
	template <typename CLOSURE_TYPE, typename INPUT_TYPE>
	struct CallbackT
	{
		void (*Function)(CLOSURE_TYPE* p_closure, INPUT_TYPE* p_input);
		CLOSURE_TYPE* Closure;
	};
}