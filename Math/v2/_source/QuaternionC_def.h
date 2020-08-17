#pragma once

#include "VectorC_def.h"

typedef struct QUATERNION4F_TYP
{
	union
	{
		struct { float x,y,z,w; };
		VECTOR4F Points;
		struct { VECTOR3F Vec; float Scal; };
	};
} QUATERNION4F, *QUATERNION4F_PTR;
