#include "InputGlobalEvents.hpp"

namespace _GameEngine::_Input
{
	_Core::ObserverT<InputGlobalEvent> InputGlobalEvents{};

	void initializeInputGlobalEvents(_Render::Window* p_window);

	void InputGlobalEvents_initialize(_Render::Window* p_window)
	{
		_Core::ObserverT_alloc(&InputGlobalEvents);
		initializeInputGlobalEvents(p_window);
	};

	void InputGlobalEvents_free()
	{
		_Core::ObserverT_free(&InputGlobalEvents);
	};


}