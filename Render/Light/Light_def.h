#pragma once

#include "v2/_interface/VectorC_def.h"

typedef struct DirectionalLight_TYP
{
	Vector3f Direction;
	float Intensity;
}DirectionalLight, * DirectionalLight_PTR;