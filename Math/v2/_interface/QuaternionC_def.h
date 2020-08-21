#pragma once

#include "v2/_interface/VectorC_def.h"

typedef struct Quaternion4f_TYP
{
	union
	{
		struct { float x,y,z,w; };
		Vector4f Points;
		struct { Vector3f Vec; float Scal; };
	};
} Quaternion4f, *Quaternion4f_PTR;
