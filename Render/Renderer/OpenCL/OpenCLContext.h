#pragma once

#if HAS_OPENCL

#include "OpenCLContext_def.h"

OpenCLKernel TransformKernel;
OpenCLKernel PolygonNormalAndBackfaceCullKernel;

void OpenCLContext_Initialize(OpenCLContext_PTR p_openclContext);

#endif