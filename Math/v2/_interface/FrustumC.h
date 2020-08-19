#pragma once

#include "FrustumC_def.h"
#include "MatrixC_def.h"

void Frustum_ExtractFromProjection(const MATRIX4F_PTR p_projection, FRUSTUM_PTR out_frustum);