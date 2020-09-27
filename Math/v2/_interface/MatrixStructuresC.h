#pragma once

#include "MatrixStructuresC_def.h"
#include "DataStructures/ARRAY.h"

ARRAY_ALLOC_FUNCTION(Matrix4f, Array_Matrix4f_PTR, Matrix4f)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(Matrix4f, Array_Matrix4f_PTR, Matrix4f)

ARRAY_ALLOC_FUNCTION(Matrix4fPTR, Array_Matrix4fPTR_PTR, Matrix4f_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(Matrix4fPTR, Array_Matrix4fPTR_PTR, Matrix4f_PTR)