#pragma once

#include <stdbool.h>

#define CORE_EQUALS(LeftTypeName, RightTypeName) Core_Equals_##LeftTypeName##_##RightTypeName

bool CORE_EQUALS(float, float)(float* p_left, float* p_right);
