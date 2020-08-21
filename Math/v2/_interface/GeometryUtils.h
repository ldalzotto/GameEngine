#pragma once

#include "VectorC_def.h"

void Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(const Vector3f_PTR p_constraint_normal, const Vector3f_PTR p_constraint_normalPoint, const Vector3f_PTR p_lookAtPoint,
	Vector3f_PTR out_lookAtDirection);