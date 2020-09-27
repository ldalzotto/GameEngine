#include <stdio.h>
#include <stdlib.h>

#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/VectorStructuresC_def.h"

inline void Assert_Equals_Sizet(size_t p_left, size_t p_right, const char* p_message)
{
	if (p_left != p_right)
	{
		if (p_message) { printf(p_message); }
		abort();
	}
}

inline void Assert_Contains_Vector2i(Array_Vector2i_PTR p_vec, Vector2i_PTR p_item, const char* p_message)
{
	for (size_t i = 0; i < p_vec->Size; i++)
	{
		if (p_vec->Memory[i].x == p_item->x && p_vec->Memory[i].y == p_item->y)
		{
			return;
		}
	}

	if (p_message) { printf(p_message); }
	abort();
}


inline void Assert_Contains_Vector2iArr(Array_Vector2i_PTR p_vec, Array_Vector2i_PTR p_item, const char* p_message)
{
	for (size_t i = 0; i < p_item->Size; i++)
	{
		for (size_t j = 0; j < p_vec->Size; j++)
		{
			if (p_vec->Memory[j].x == p_item->Memory[i].x && p_vec->Memory[j].y == p_item->Memory[i].y)
			{
				goto continue_lab;
			}
		}

		if (p_message) { printf(p_message); }
		abort();

		continue_lab:;
	}
}