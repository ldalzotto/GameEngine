#pragma once

#include <stdbool.h>
#include "v2/_interface/VectorC_def.h"
#include "Objects/Resource/Polygon_def.h"

bool BackFaceCulled_Poly4F(const Polygon4f_PTR p_polygon, const Vector4f_PTR p_cameraWorldPosition);
bool BackFaceCulled_Poly4FPTR(const Polygon4fPTR_PTR p_polygon, const Vector4f_PTR p_cameraWorldPosition);