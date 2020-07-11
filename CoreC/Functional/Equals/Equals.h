#pragma once

#include <stdbool.h>

#define CORE_EQUALS(LeftTypeName, RightTypeName) Core_Equals_##LeftTypeName##_##RightTypeName

bool CORE_EQUALS(float, float)(float* p_left, float* p_right);
bool CORE_EQUALS(uint16, uint16)(unsigned __int16* p_left, unsigned __int16* p_right);
bool CORE_EQUALS(sizet, sizet)(size_t* p_left, size_t* p_right);