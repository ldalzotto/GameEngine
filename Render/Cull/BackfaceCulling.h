#pragma once

#include "v2/_interface/VectorC_def.h"
#include "Objects/Resource/Polygon_def.h"

char BackFaceCulled_Poly4FPTR(const Polygon4fPTR_PTR p_polygon, const Vector4f_PTR p_cameraWorldPosition);
char BackFaceCulled_Normal3fPTR(const Vector3f_PTR p_worldNormal, const Vector4f_PTR p_polygonWorldPosition, const Vector4f_PTR p_cameraWorldPosition);