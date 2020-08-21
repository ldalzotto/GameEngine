#pragma once

#include "FrustumC_def.h"
#include "MatrixC_def.h"

void Frustum_ExtractFromProjection(const Matrix4f_PTR p_projection, Frustum_PTR out_frustum);