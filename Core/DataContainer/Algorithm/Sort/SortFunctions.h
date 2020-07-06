#pragma once

#include <stdint.h>

#define SORT_COMPARTOR_FUNCTION_NAME(ComparedType) Core_##ComparedType##SortCompararator
#define SORT_COMPARTOR_FUNCTION_DEFINE(ComparedType) short SORT_COMPARTOR_FUNCTION_NAME(ComparedType)(##ComparedType p_left, ##ComparedType p_right);

SORT_COMPARTOR_FUNCTION_DEFINE(size_t)
SORT_COMPARTOR_FUNCTION_DEFINE(uint16_t)
SORT_COMPARTOR_FUNCTION_DEFINE(float)
