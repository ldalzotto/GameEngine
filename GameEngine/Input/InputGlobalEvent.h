#pragma once

#include "DataStructures/ARRAY_def.h"

typedef unsigned char InputKey;
#define InputKey_SPACE 0
#define InputKey_APOSTROPHE 1
#define InputKey_COMMA 2
#define InputKey_MINUS 3
#define InputKey_PERIOD 4
#define InputKey_SLASH 5
#define InputKey_ZERO 6
#define InputKey_ONE 7
#define InputKey_TWO 8
#define InputKey_THREE 9
#define InputKey_FOUR 10
#define InputKey_FIVE 11
#define InputKey_SIX 12
#define InputKey_SEVEN 13
#define InputKey_EIGHT 14
#define InputKey_NINE 15
#define InputKey_SEMICOLON 16
#define InputKey_EQUAL 17
#define InputKey_A 18
#define InputKey_B 19
#define InputKey_C 20
#define InputKey_D 21
#define InputKey_E 22
#define InputKey_F 23
#define InputKey_G 24
#define InputKey_H 25
#define InputKey_I 26
#define InputKey_J 27
#define InputKey_K 28
#define InputKey_L 29
#define InputKey_M 30
#define InputKey_N 31
#define InputKey_O 32
#define InputKey_P 33
#define InputKey_Q 34
#define InputKey_R 35
#define InputKey_S 36
#define InputKey_T 37
#define InputKey_U 38
#define InputKey_V 39
#define InputKey_W 40
#define InputKey_X 41
#define InputKey_Y 42
#define InputKey_Z 43
#define InputKey_LEFT_BRACKET 44
#define InputKey_BACKSLASH 45
#define InputKey_RIGHT_BRACKET 46
#define InputKey_GRAVE_ACCENT 47
#define InputKey_WORLD_1 48
#define InputKey_WORLD_2 49
#define InputKey_ESCAPE 50
#define InputKey_ENTER 51
#define InputKey_TAB 52
#define InputKey_BACKSPACE 53
#define InputKey_INSERT 54
#define InputKey_DELETE_2 55
#define InputKey_RIGHT 56
#define InputKey_LEFT 57
#define InputKey_DOWN 58
#define InputKey_UP 59
#define InputKey_PAGE_UP 60
#define InputKey_PAGE_DOWN 61
#define InputKey_HOME 62
#define InputKey_END 63
#define InputKey_CAPS_LOCK 64
#define InputKey_SCROLL_LOCK 65
#define InputKey_NUM_LOCK 66
#define InputKey_PRINT_SCREEN 67
#define InputKey_PAUSE 68
#define InputKey_F1 69
#define InputKey_F2 70
#define InputKey_F3 71
#define InputKey_F4 72
#define InputKey_F5 73
#define InputKey_F6 74
#define InputKey_F7 75
#define InputKey_F8 76
#define InputKey_F9 77
#define InputKey_F10 78
#define InputKey_F11 79
#define InputKey_F12 80
#define InputKey_F13 81
#define InputKey_F14 82
#define InputKey_F15 83
#define InputKey_F16 84
#define InputKey_F17 85
#define InputKey_F18 86
#define InputKey_F19 87
#define InputKey_F20 88
#define InputKey_F21 89
#define InputKey_F22 90
#define InputKey_F23 91
#define InputKey_F24 92
#define InputKey_F25 93
#define InputKey_KP_ZERO 94
#define InputKey_KP_ONE 95
#define InputKey_KP_TWO 96
#define InputKey_KP_THREE 97
#define InputKey_KP_FOUR 98
#define InputKey_KP_FIVE 99
#define InputKey_KP_SIX 100
#define InputKey_KP_SEVEN 101
#define InputKey_KP_EIGHT 102
#define InputKey_KP_NINE 103
#define InputKey_KP_DECIMAL 104
#define InputKey_KP_DIVIDE 105
#define InputKey_KP_MULTIPLY 106
#define InputKey_KP_SUBTRACT 107
#define InputKey_KP_ADD 108
#define InputKey_KP_ENTER 109
#define InputKey_KP_EQUAL 110
#define InputKey_LEFT_SHIFT 111
#define InputKey_LEFT_CONTROL 112
#define InputKey_LEFT_ALT 113
#define InputKey_LEFT_SUPER 114
#define InputKey_RIGHT_SHIFT 115
#define InputKey_RIGHT_CONTROL 116
#define InputKey_RIGHT_ALT 117
#define InputKey_RIGHT_SUPER 118
#define InputKey_MENU 119
#define InputKey_MOUSE_BUTTON_1 120
#define InputKey_MOUSE_BUTTON_2 121
#define InputKey_MOUSE_BUTTON_3 122
#define InputKey_MOUSE_BUTTON_4 123
#define InputKey_MOUSE_BUTTON_5 124
#define InputKey_MOUSE_BUTTON_6 125
#define InputKey_MOUSE_BUTTON_7 126
#define InputKey_MOUSE_BUTTON_8 127
#define InputKey_MOUSE_BUTTON_LAST 128
#define InputKey_MOUSE_BUTTON_LEFT 129
#define InputKey_MOUSE_BUTTON_RIGHT 130
#define InputKey_MOUSE_BUTTON_MIDDLE 131
#define InputKey_LAST 132

typedef struct Array_InputKey_TYP
{
	ARRAY_TYPE_DEFINITION(InputKey)
} Array_InputKey, * Array_InputKey_PTR;

typedef char InputGlobalEventType;
#define InputGlobalEventType_PRESSED 0
#define InputGlobalEventType_RELEASED 1
#define InputGlobalEventType_REPEAT 2

typedef struct InputGlobalEvent_TYP
{
	InputKey Key;
	InputGlobalEventType Type;
}InputGlobalEvent, * InputGlobalEvent_PTR;

typedef struct Array_InputGlobalEvent_TYP
{
	ARRAY_TYPE_DEFINITION(InputGlobalEvent)
}Array_InputGlobalEvent, * Array_InputGlobalEvent_PTR;

typedef struct InputMouseGlobalEvent_TYP
{
	int X; int Y;
}InputMouseGlobalEvent, * InputMouseGlobalEvent_PTR;

extern Array_InputGlobalEvent GInputGlobalEvents;
extern char GInputMouseGlobalEvent_UpdatedThisFrame;
extern InputMouseGlobalEvent GInputMouseGlobalEvent;

void InputGlobalEvent_Initialize();
void InputGLobalEvent_ConsumeEvents();
void InputGlobalEvent_Free();