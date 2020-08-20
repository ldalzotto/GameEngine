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
extern "C"
{
#include "AppEvent/AppEvent.h"
}
namespace _GameEngine::_Input
{
	void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, AppEvent_Header* p_eventHeader);

	void inputGlobalEventPlatformSpecific_initialize()
	{
		AppEventObserver_Register(&EventDispatcher, (void(*)(void*, AppEvent_Header*))inputGlobalEventPlatformSpecific_handleInputEvent, nullptr);
	};

	void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, AppEvent_Header* p_eventHeader)
	{
		if (*p_eventHeader == AppEventType_INPUT_KEY_EVENT)
		{
			InputKeyEvent* l_inputKeyEvent = (InputKeyEvent*)p_eventHeader;
			InputGlobalEvent l_inputGlobalEvent;
			l_inputGlobalEvent.Type = static_cast<InputGlobalEventType>(l_inputKeyEvent->Type);

			//see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
			switch (l_inputKeyEvent->Key)
			{
			case 0x41:
			{
				l_inputGlobalEvent.Key = InputKey::A;
			}
			break;
			case 0x42:
			{
				l_inputGlobalEvent.Key = InputKey::B;
			}
			break;
			case 0x43:
			{
				l_inputGlobalEvent.Key = InputKey::C;
			}
			break;
			case 0x44:
			{
				l_inputGlobalEvent.Key = InputKey::D;
			}
			break;
			case 0x45:
			{
				l_inputGlobalEvent.Key = InputKey::E;
			}
			break;
			case 0x46:
			{
				l_inputGlobalEvent.Key = InputKey::F;
			}
			break;
			case 0x47:
			{
				l_inputGlobalEvent.Key = InputKey::G;
			}
			break;
			case 0x48:
			{
				l_inputGlobalEvent.Key = InputKey::H;
			}
			break;
			case 0x49:
			{
				l_inputGlobalEvent.Key = InputKey::I;
			}
			break;
			case 0x4A:
			{
				l_inputGlobalEvent.Key = InputKey::J;
			}
			break;
			case 0x4B:
			{
				l_inputGlobalEvent.Key = InputKey::K;
			}
			break;
			case 0x4C:
			{
				l_inputGlobalEvent.Key = InputKey::L;
			}
			break;
			case 0x4D:
			{
				l_inputGlobalEvent.Key = InputKey::M;
			}
			break;
			case 0x4E:
			{
				l_inputGlobalEvent.Key = InputKey::N;
			}
			break;
			case 0x4F:
			{
				l_inputGlobalEvent.Key = InputKey::O;
			}
			break;
			case 0x50:
			{
				l_inputGlobalEvent.Key = InputKey::P;
			}
			break;
			case 0x51:
			{
				l_inputGlobalEvent.Key = InputKey::Q;
			}
			break;
			case 0x52:
			{
				l_inputGlobalEvent.Key = InputKey::R;
			}
			break;
			case 0x53:
			{
				l_inputGlobalEvent.Key = InputKey::S;
			}
			break;
			case 0x54:
			{
				l_inputGlobalEvent.Key = InputKey::T;
			}
			break;
			case 0x55:
			{
				l_inputGlobalEvent.Key = InputKey::U;
			}
			break;
			case 0x56:
			{
				l_inputGlobalEvent.Key = InputKey::V;
			}
			break;
			case 0x57:
			{
				l_inputGlobalEvent.Key = InputKey::W;
			}
			break;
			case 0x58:
			{
				l_inputGlobalEvent.Key = InputKey::X;
			}
			break;
			case 0x59:
			{
				l_inputGlobalEvent.Key = InputKey::Y;
			}
			break;
			case 0x5A:
			{
				l_inputGlobalEvent.Key = InputKey::Z;
			}
			break;
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
		else if (*p_eventHeader == AppEventType_INPUT_MOUSE_EVENT)
		{
			InputMouseEvent* l_inputMouseEvent = (InputMouseEvent*)p_eventHeader;
			InputMouseGlobalEvent l_inputMouseGlobalEvent;
			l_inputMouseGlobalEvent.X = l_inputMouseEvent->X;
			l_inputMouseGlobalEvent.Y = l_inputMouseEvent->Y;

			_Core::ObserverT_broadcast(&InputMouseGlobalEvent_dispatcher, &l_inputMouseGlobalEvent);
		}
	};
}
#endif