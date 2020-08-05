#include "InputGlobalEvent.hpp"

namespace _GameEngine::_Input
{
	void inputGlobalEventPlatformSpecific_initialize();

	_Core::ObserverT<InputGlobalEvent> InputGlobalEvent_dispatcher{};
	_Core::ObserverT<InputMouseGlobalEvent> InputMouseGlobalEvent_dispatcher{};

	void InputGlobalEvent_initialize()
	{
		_Core::ObserverT_alloc(&InputGlobalEvent_dispatcher);
		_Core::ObserverT_alloc(&InputMouseGlobalEvent_dispatcher);
		inputGlobalEventPlatformSpecific_initialize();
	};

	void InputGlobalEvent_free()
	{
		_Core::ObserverT_free(&InputGlobalEvent_dispatcher);
		_Core::ObserverT_free(&InputMouseGlobalEvent_dispatcher);
	};
}

#ifdef _WIN32
#include "AppEvent/AppEvent.hpp"
namespace _GameEngine::_Input
{
	void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, _Core::AppEvent_Header* p_eventHeader);

	void inputGlobalEventPlatformSpecific_initialize()
	{
		_Core::CallbackT<void, _Core::AppEvent_Header> l_callback = { inputGlobalEventPlatformSpecific_handleInputEvent, nullptr };
		_Core::ObserverT_register(&_Core::EventDispatcher, &l_callback);
	};

	void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, _Core::AppEvent_Header* p_eventHeader)
	{
		if (p_eventHeader->EventType == _Core::AppEventType::INPUT_KEY_EVENT)
		{
			_Core::InputKeyEvent* l_inputKeyEvent = (_Core::InputKeyEvent*)p_eventHeader;
			InputGlobalEvent l_inputGlobalEvent;
			l_inputGlobalEvent.Type = static_cast<InputGlobalEventType>(l_inputKeyEvent->Type);

			switch (l_inputKeyEvent->Key)
			{
			case WM_LBUTTONDOWN:
			{
				l_inputGlobalEvent.Key = InputKey::MOUSE_BUTTON_1;
			}
			break;
			case WM_LBUTTONUP:
			{
				l_inputGlobalEvent.Key = InputKey::MOUSE_BUTTON_1;
			}
			break;
			default:
				return;
			}

			_Core::ObserverT_broadcast(&InputGlobalEvent_dispatcher, &l_inputGlobalEvent);
		}
		else if (p_eventHeader->EventType == _Core::AppEventType::INPUT_MOUSE_EVENT)
		{
			_Core::InputMouseEvent* l_inputMouseEvent = (_Core::InputMouseEvent*)p_eventHeader;
			InputMouseGlobalEvent l_inputMouseGlobalEvent;
			l_inputMouseGlobalEvent.X = l_inputMouseEvent->X; 
			l_inputMouseGlobalEvent.Y = l_inputMouseEvent->Y;

			_Core::ObserverT_broadcast(&InputMouseGlobalEvent_dispatcher, &l_inputMouseGlobalEvent);
		}
	};
}
#endif