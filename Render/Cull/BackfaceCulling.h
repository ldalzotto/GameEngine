#pragma once

#include <stdbool.h>
#include "v2/_interface/VectorC_def.h"
#include "Objects/Resource/Polygon_def.h"

bool BackFaceCulled_Poly4F(const POLYGON4F_PTR p_polygon, const VECTOR4F_PTR p_cameraWorldPosition);
