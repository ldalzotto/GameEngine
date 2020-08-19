#include "v2/_interface/GeometryUtils.h"

#include "v2/_interface/PlaneC.h"
#include "v2/_interface/VectorC.h"

void Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(const VECTOR3F_PTR p_constraint_normal, const VECTOR3F_PTR p_constraint_normalPoint, const VECTOR3F_PTR p_lookAtPoint,
	VECTOR3F_PTR out_lookAtDirection)
{
	PLANE l_perpendicularPlaneToNormal;
	Plane_Build_Normal_Point(p_constraint_normal, p_constraint_normalPoint, &l_perpendicularPlaneToNormal);
	VECTOR3F l_projectedPoint;
	Plane_ProjectPoint(&l_perpendicularPlaneToNormal, p_lookAtPoint, &l_projectedPoint);
	Vec_Min_3f_3f(&l_projectedPoint, p_constraint_normalPoint, out_lookAtDirection);
	Vec_Normalize_3f(out_lookAtDirection, out_lookAtDirection);
};