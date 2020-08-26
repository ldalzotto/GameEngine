#include "InputGlobalEvent.h"
#include "DataStructures/ARRAY.h"
#include "AppEvent/AppEvent.h"

#include "v2/Math.h"
#include "v2/_interface/VectorC.h"
#include "Objects/Window/Window.h"

ARRAY_ALLOC_FUNCTION(InputGlobalEvent, Array_InputGlobalEvent_PTR, InputGlobalEvent);
ARRAY_FREE_FUNCTION(InputGlobalEvent, Array_InputGlobalEvent_PTR);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(InputGlobalEvent, Array_InputGlobalEvent_PTR, InputGlobalEvent);
ARRAY_COPYTOREALLOC_FUNCTION(InputGlobalEvent, Array_InputGlobalEvent_PTR, InputGlobalEvent);

ARRAY_ALLOC_FUNCTION(InputKey, Array_InputKey_PTR, InputKey)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(InputKey, Array_InputKey_PTR, InputKey)
ARRAY_FREE_FUNCTION(InputKey, Array_InputKey_PTR)

Array_InputGlobalEvent GInputGlobalEvents = { 0 };
char GInputMouseGlobalEvent_UpdatedThisFrame = 0;
InputMouseGlobalEvent GInputMouseGlobalEvent = { 0 };

void inputGlobalEventPlatformSpecific_initialize();

void InputGlobalEvent_Initialize()
{
	Arr_Alloc_InputGlobalEvent(&GInputGlobalEvents, 0);
	inputGlobalEventPlatformSpecific_initialize();
};

void InputGLobalEvent_ConsumeEvents()
{
	Arr_Clear(&GInputGlobalEvents.array);
	GInputMouseGlobalEvent_UpdatedThisFrame = 0;
};

void InputGlobalEvent_Free()
{
	Arr_Free_InputGlobalEvent(&GInputGlobalEvents);
};

#ifdef _WIN32
void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, AppEvent_Header* p_eventHeader);

void inputGlobalEventPlatformSpecific_initialize()
{
	AppEventObserver_Register(&EventDispatcher, (void(*)(void*, AppEvent_Header*))inputGlobalEventPlatformSpecific_handleInputEvent, NULL);
};

void inputGlobalEventPlatformSpecific_handleInputEvent(void* p_null, AppEvent_Header* p_eventHeader)
{
	if (*p_eventHeader == AppEventType_INPUT_KEY_EVENT)
	{
		InputKeyEvent* l_inputKeyEvent = (InputKeyEvent*)p_eventHeader;
		InputGlobalEvent l_inputGlobalEvent;
		l_inputGlobalEvent.Type = (InputGlobalEventType)l_inputKeyEvent->Type;

		//see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		switch (l_inputKeyEvent->Key)
		{
		case 0x41:
		{
			l_inputGlobalEvent.Key = InputKey_A;
		}
		break;
		case 0x42:
		{
			l_inputGlobalEvent.Key = InputKey_B;
		}
		break;
		case 0x43:
		{
			l_inputGlobalEvent.Key = InputKey_C;
		}
		break;
		case 0x44:
		{
			l_inputGlobalEvent.Key = InputKey_D;
		}
		break;
		case 0x45:
		{
			l_inputGlobalEvent.Key = InputKey_E;
		}
		break;
		case 0x46:
		{
			l_inputGlobalEvent.Key = InputKey_F;
		}
		break;
		case 0x47:
		{
			l_inputGlobalEvent.Key = InputKey_G;
		}
		break;
		case 0x48:
		{
			l_inputGlobalEvent.Key = InputKey_H;
		}
		break;
		case 0x49:
		{
			l_inputGlobalEvent.Key = InputKey_I;
		}
		break;
		case 0x4A:
		{
			l_inputGlobalEvent.Key = InputKey_J;
		}
		break;
		case 0x4B:
		{
			l_inputGlobalEvent.Key = InputKey_K;
		}
		break;
		case 0x4C:
		{
			l_inputGlobalEvent.Key = InputKey_L;
		}
		break;
		case 0x4D:
		{
			l_inputGlobalEvent.Key = InputKey_M;
		}
		break;
		case 0x4E:
		{
			l_inputGlobalEvent.Key = InputKey_N;
		}
		break;
		case 0x4F:
		{
			l_inputGlobalEvent.Key = InputKey_O;
		}
		break;
		case 0x50:
		{
			l_inputGlobalEvent.Key = InputKey_P;
		}
		break;
		case 0x51:
		{
			l_inputGlobalEvent.Key = InputKey_Q;
		}
		break;
		case 0x52:
		{
			l_inputGlobalEvent.Key = InputKey_R;
		}
		break;
		case 0x53:
		{
			l_inputGlobalEvent.Key = InputKey_S;
		}
		break;
		case 0x54:
		{
			l_inputGlobalEvent.Key = InputKey_T;
		}
		break;
		case 0x55:
		{
			l_inputGlobalEvent.Key = InputKey_U;
		}
		break;
		case 0x56:
		{
			l_inputGlobalEvent.Key = InputKey_V;
		}
		break;
		case 0x57:
		{
			l_inputGlobalEvent.Key = InputKey_W;
		}
		break;
		case 0x58:
		{
			l_inputGlobalEvent.Key = InputKey_X;
		}
		break;
		case 0x59:
		{
			l_inputGlobalEvent.Key = InputKey_Y;
		}
		break;
		case 0x5A:
		{
			l_inputGlobalEvent.Key = InputKey_Z;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			l_inputGlobalEvent.Key = InputKey_MOUSE_BUTTON_1;
		}
		break;
		case WM_LBUTTONUP:
		{
			l_inputGlobalEvent.Key = InputKey_MOUSE_BUTTON_1;
		}
		break;
		default:
			return;
		}

		Arr_PushBackRealloc_InputGlobalEvent(&GInputGlobalEvents, &l_inputGlobalEvent);
	}
	else if (*p_eventHeader == AppEventType_INPUT_MOUSE_EVENT)
	{
		InputMouseEvent* l_inputMouseEvent = (InputMouseEvent*)p_eventHeader;
		GInputMouseGlobalEvent.X = l_inputMouseEvent->X;
		GInputMouseGlobalEvent.Y = l_inputMouseEvent->Y;
		GInputMouseGlobalEvent_UpdatedThisFrame = 1;
	}
};

#endif

#include "InputV2.h"

ARRAY_ALLOC_FUNCTION(KeyStateFlag, Array_KeyStateFlag_PTR, KeyStateFlag);
ARRAY_FREE_FUNCTION(KeyStateFlag, Array_KeyStateFlag_PTR);
ARRAY_ZEROING_FUNCTION(KeyStateFlag, Array_KeyStateFlag_PTR, KeyStateFlag);

Segment_Vector2f InputMouse_getMouseDeltaScreenPosition(InputMouse* p_inputMouse)
{
	Segment_Vector2f l_segment;
	l_segment.Begin = (Vector2f) { (float)p_inputMouse->LastFrameMouseAbsoluteScreenPosition.x, (float)p_inputMouse->LastFrameMouseAbsoluteScreenPosition.y };
	l_segment.End = (Vector2f){ (float)p_inputMouse->ScreenPosition.x, (float)p_inputMouse->ScreenPosition.y };
	return l_segment;
};

void Input_build(Input* p_input, Window* p_window, Log_PTR Log)
{
	p_input->Window = p_window;
	p_input->Log = Log;

	p_input->InputMouse = (InputMouse) { 0 };
	p_input->InputMouse.MouseEnabled = true;
	p_input->InputMouse.MouseSentitivityperPixel = 0.01f;

	Arr_Alloc_KeyStateFlag(&p_input->InputState, (size_t)InputKey_LAST);
	Arr_Zeroing_KeyStateFlag(&p_input->InputState);
	Arr_Clear(&p_input->InputState.array);

	Arr_Alloc_InputKey(&p_input->InputKeysJustPressedThisFrame, 0);
	Arr_Alloc_InputKey(&p_input->InputKeysReleasedThisFrame, 0);
	Arr_Alloc_InputGlobalEvent(&p_input->InputEventsLastFrame, 0);
};

void Input_handleEvent(Input_PTR p_input, InputGlobalEvent_PTR p_inputEvent)
{
	KeyStateFlag* l_oldStateFlag = &p_input->InputState.Memory[(size_t)p_inputEvent->Key];

	if (((*l_oldStateFlag == KeyStateFlag_PRESSED) || (*l_oldStateFlag == KeyStateFlag_PRESSED_THIS_FRAME)) && p_inputEvent->Type == InputGlobalEventType_RELEASED)
	{
		*l_oldStateFlag = KeyStateFlag_RELEASED_THIS_FRAME;
		Arr_PushBackRealloc_InputKey(&p_input->InputKeysReleasedThisFrame, &p_inputEvent->Key);
	}
	else if ((*l_oldStateFlag == KeyStateFlag_PRESSED_THIS_FRAME) && (p_inputEvent->Type== InputGlobalEventType_REPEAT || p_inputEvent->Type == InputGlobalEventType_PRESSED))
	{
		*l_oldStateFlag = KeyStateFlag_PRESSED;
	}
	else if (((*l_oldStateFlag == KeyStateFlag_NONE) || (*l_oldStateFlag == KeyStateFlag_RELEASED_THIS_FRAME)) && p_inputEvent->Type == InputGlobalEventType_PRESSED)
	{
		*l_oldStateFlag = KeyStateFlag_PRESSED_THIS_FRAME;
		Arr_PushBackRealloc_InputKey(&p_input->InputKeysJustPressedThisFrame, &p_inputEvent->Key);
	}
}

void Input_consumeInputEvents(Input_PTR p_input)
{
	for (size_t i = 0; i < GInputGlobalEvents.Size; i++)
	{
		Input_handleEvent(p_input, &GInputGlobalEvents.Memory[i]);
	}

	for (size_t i = 0; i < p_input->InputEventsLastFrame.Size; i++)
	{
		Input_handleEvent(p_input, &p_input->InputEventsLastFrame.Memory[i]);
	}

	Arr_Clear(&p_input->InputEventsLastFrame.array);
};

void Input_free(Input* p_input)
{
	Arr_Free_KeyStateFlag(&p_input->InputState);
	Arr_Free_InputKey(&p_input->InputKeysJustPressedThisFrame);
	Arr_Free_InputKey(&p_input->InputKeysReleasedThisFrame);
	Arr_Free_InputGlobalEvent(&p_input->InputEventsLastFrame);
};

bool Input_getState(Input* p_input, InputKey p_key, KeyStateFlag p_keyStateFlag)
{
	return p_input->InputState.Memory[(int)p_key] & p_keyStateFlag;
};

void Input_newFrame(Input* p_input)
{
	if (p_input->InputKeysReleasedThisFrame.Size > 0)
	{
		for (size_t i = 0; i < p_input->InputKeysReleasedThisFrame.Size; i++)
		{
			p_input->InputState.Memory[p_input->InputKeysReleasedThisFrame.Memory[i]] = KeyStateFlag_NONE;
		}

		Arr_Clear(&p_input->InputKeysReleasedThisFrame.array);
	}

	for (size_t i = 0; i < p_input->InputKeysJustPressedThisFrame.Size; i++)
	{
		InputKey l_inputKeyCode = p_input->InputKeysJustPressedThisFrame.Memory[i];
		
		InputGlobalEvent l_event = (InputGlobalEvent){.Key = l_inputKeyCode , .Type = InputGlobalEventType_REPEAT};
		Arr_PushBackRealloc_InputGlobalEvent(&p_input->InputEventsLastFrame, &l_event);
	}
	Arr_Clear(&p_input->InputKeysJustPressedThisFrame.array);

	Input_consumeInputEvents(p_input);

	if (GInputMouseGlobalEvent_UpdatedThisFrame)
	{
		p_input->InputMouse.LastFrameMouseAbsoluteScreenPosition = p_input->InputMouse.ScreenPosition;
		p_input->InputMouse.ScreenPosition.x = GInputMouseGlobalEvent.X;
		p_input->InputMouse.ScreenPosition.y = GInputMouseGlobalEvent.Y;

		// std::cout << "X" << p_input->InputMouse.ScreenPosition.x << "  Y  " << p_input->InputMouse.ScreenPosition.y << std::endl;

		Vec_Min_2d_2d(&p_input->InputMouse.ScreenPosition, &p_input->InputMouse.LastFrameMouseAbsoluteScreenPosition, &p_input->InputMouse.MouseDelta);

		WindowSize l_windowDimensions = Window_getSize(p_input->Window);

		p_input->InputMouse.ScreenPosition.x = Math_max(Math_min(p_input->InputMouse.ScreenPosition.x, (double)l_windowDimensions.Width), 0.0f);
		p_input->InputMouse.ScreenPosition.y = Math_max(Math_min(p_input->InputMouse.ScreenPosition.y, (double)l_windowDimensions.Height), 0.0f);

	}
	else
	{
		p_input->InputMouse.MouseDelta = (Vector2d) { 0.0, 0.0 };
	}

	InputGLobalEvent_ConsumeEvents();
};
