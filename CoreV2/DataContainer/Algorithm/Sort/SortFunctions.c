#pragma message (__FILE__)

#define SORT_COMPARTOR_FUNCTION_DEFINE_IMPL(ComparedType) \
	short SORT_COMPARTOR_FUNCTION_NAME(ComparedType)(##ComparedType p_left, ##ComparedType p_right) \
	{ \
		if (p_left > p_right) { return 1; } \
		else if (p_left < p_right) { return -1; } \
		else { return 0; } \
	};

SORT_COMPARTOR_FUNCTION_DEFINE_IMPL(size_t)
SORT_COMPARTOR_FUNCTION_DEFINE_IMPL(uint16_t)
SORT_COMPARTOR_FUNCTION_DEFINE_IMPL(float)