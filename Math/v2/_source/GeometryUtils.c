#include "v2/_interface/GeometryUtils.h"

#include "v2/_interface/PlaneC.h"
#include "v2/_interface/VectorC.h"

void Geom_LookAtPoint_DirectionVector_PerpendicularToNormal(const Vector3f_PTR p_constraint_normal, const Vector3f_PTR p_constraint_normalPoint, const Vector3f_PTR p_lookAtPoint,
	Vector3f_PTR out_lookAtDirection)
{
	Plane l_perpendicularPlaneToNormal;
	Plane_Build_Normal_Point(p_constraint_normal, p_constraint_normalPoint, &l_perpendicularPlaneToNormal);
	Vector3f l_projectedPoint;
	Plane_ProjectPoint(&l_perpendicularPlaneToNormal, p_lookAtPoint, &l_projectedPoint);
	Vec_Min_3f_3f(&l_projectedPoint, p_constraint_normalPoint, out_lookAtDirection);
	Vec_Normalize_3f(out_lookAtDirection, out_lookAtDirection);
};