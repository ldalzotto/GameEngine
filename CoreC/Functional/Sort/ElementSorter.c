#include "ElementSorter.h"

#include "Constants.h"

#define DEFINE_SORT_NUMERIC(LeftTypeName, RightTypeName) \
short int Core_sortCompare_##LeftTypeName##_##RightTypeName(LeftTypeName* p_left, RightTypeName* p_right) \
	{ \
		if  ((*p_left - *p_right) >= FLOAT_TOLERANCE) { return 1; } \
		else if ((*p_left -*p_right) <= FLOAT_TOLERANCE ) { return -1; } \
		else { return 0; } \
	};

DEFINE_SORT_NUMERIC(float, float);