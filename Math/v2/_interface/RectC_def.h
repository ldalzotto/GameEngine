#pragma once

#include "VectorC_def.h"

typedef struct Recti_TYP
{
	Vector2i Min;
	Vector2i Max;
} Recti, * Recti_PTR;

typedef char RECT_INTERSECTION_TEST;
#define RECT_INTERSECTION_TEST_NOTHING 0
#define RECT_INTERSECTION_TEST_INTERSECTION 1
#define RECT_INTERSECTION_TEST_CONTAINS 2