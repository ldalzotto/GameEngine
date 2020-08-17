#pragma once

#include "VectorC_def.h"

typedef struct MATRIX3F_TYP
{
	union
	{
		float Points[9];
		struct { float _00, _01, _02, _10, _11, _12, _20, _21, _22; };
		struct { VECTOR3F Col0, Col1, Col2; };
	};
} MATRIX3F, * MATRIX3F_PTR;

typedef struct MATRIX4F_TYP
{
	union
	{
		float Points[16];
		struct { float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		struct { VECTOR4F Col0, Col1, Col2, Col3; };
	};
} MATRIX4F, * MATRIX4F_PTR;