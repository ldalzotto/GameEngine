#pragma once

#include "VectorC_def.h"

void Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(const VECTOR3F_PTR p_constraint_normal, const VECTOR3F_PTR p_constraint_normalPoint, const VECTOR3F_PTR p_lookAtPoint,
	VECTOR3F_PTR out_lookAtDirection);