#pragma once

#include <stdbool.h>

typedef bool(*Core_Equals_function)(void* p_left, void* p_right);

#define CORE_EQUALS(LeftTypeName, RightTypeName) Core_Equals_##LeftTypeName##_##RightTypeName

bool CORE_EQUALS(voidP, voidP)(void** p_left, void** p_right);
bool CORE_EQUALS(float, float)(float* p_left, float* p_right);
bool CORE_EQUALS(uint16, uint16)(unsigned __int16* p_left, unsigned __int16* p_right);
bool CORE_EQUALS(sizet, sizet)(size_t* p_left, size_t* p_right);