#pragma once

#include "PlaneC_def.h"

// /!\ Normals are pointing inward
typedef struct Frustum_TYP
{
	Plane Left, Right, Bottom, Up, Near, Far;
}Frustum, *Frustum_PTR;
