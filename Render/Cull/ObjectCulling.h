#pragma once

#include <stdbool.h>
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/FrustumC_def.h"
#include "v2/_interface/MatrixC_def.h"

bool ObjectCulled_Boxf(const BoxF_PTR p_objectBoundingBox_localSpace, const Matrix4f_PTR p_modelMatrix,
	const Matrix4f_PTR p_objectToCamera_matrix, const Frustum_PTR p_cameraFrustum);