#pragma once

#include "ColorC_def.h"
#include "DataStructures/ARRAY.h"

void Color_Convert_3F_3C(Color3f_PTR p_color3f, Color3c_PTR p_color3c);
void Color_Convert_3C_3F(Color3c_PTR p_color3c, Color3f_PTR p_color3f);

ARRAY_ALLOC_FUNCTION(Color3c, Array_Color3c_PTR, Color3c)