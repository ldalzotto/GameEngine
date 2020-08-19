#pragma once

#include "PlaneC_def.h"

typedef struct FRUSTUM_TYP
{
	PLANE Left, Right, Bottom, Up, Near, Far;
}FRUSTUM, *FRUSTUM_PTR;
