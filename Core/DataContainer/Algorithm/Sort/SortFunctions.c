#include "SortFunctions.h"

short Core_SizeTSortCompararator(size_t p_left, size_t p_right)
{
	if (p_left > p_right) { return 1; }
	else if (p_left < p_right) { return -1; }
	else { return 0; }
};

short Core_Uint16tSortComparator(uint16_t p_left, uint16_t p_right)
{
	if (p_left > p_right) { return 1; }
	else if (p_left < p_right) { return -1; }
	else { return 0; }
}

short Core_FloatSortComparator(float p_left, float p_right)
{
	if (p_left > p_right) { return 1; }
	else if (p_left < p_right) { return -1; }
	else { return 0; }
};