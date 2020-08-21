#pragma once

#include "VectorC_def.h"

typedef struct Matrix3f_TYP
{
	union
	{
		float Points[9];
		struct { float _00, _01, _02, _10, _11, _12, _20, _21, _22; };
		struct { Vector3f Col0, Col1, Col2; };
		struct { Vector3f Right, Up, Forward; };
	};
} Matrix3f, * Matrix3f_PTR;

typedef struct Matrix4f_TYP
{
	union
	{
		float Points[16];
		struct { float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		struct { Vector4f Col0, Col1, Col2, Col3; };
		struct { Vector4f Right, Up, Forward, Col3_Direction; };
	};
} Matrix4f, * Matrix4f_PTR;