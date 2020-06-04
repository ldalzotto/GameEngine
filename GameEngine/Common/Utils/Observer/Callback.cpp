#include "Callback.h"

namespace _GameEngine::_Utils
{

	void Callback_invoke(Callback* p_callback, void* p_inputParameter)
	{
		if (p_callback->Function)
		{
			p_callback->Function(p_callback->Closure, p_inputParameter);
		}
	};
}