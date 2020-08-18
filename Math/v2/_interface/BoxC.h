#pragma once

#include "VectorStructuresC_def.h"
#include "BoxC_def.h"
#include "MatrixC_def.h"

typedef struct BOXFPOINTS_TYP
{
	union 
	{
		struct
		{
			VECTOR3F L_D_F;
			VECTOR3F L_U_F;

			VECTOR3F R_D_F;
			VECTOR3F R_U_F;

			VECTOR3F L_D_B;
			VECTOR3F L_U_B;

			VECTOR3F R_D_B;
			VECTOR3F R_U_B;

			VECTOR3F Center;
		};

		VECTOR3F Points[9];
	};
	
}BOXFPOINTS, * BOXFPOINTS_PTR;

void Box_Build_F(BOXF_PTR p_box, ARRAY_VECTOR3F_PTR p_points);
void Box_ExtractPoints_F(const BOXF_PTR p_box, BOXFPOINTS_PTR p_out);
void Box_ExtractMinMax_F(const BOXF_PTR p_box, VECTOR3F_PTR out_min, VECTOR3F_PTR out_max);
void BoxPoints_Mul_F_M4F(const BOXFPOINTS_PTR p_boxPoints, const MATRIX4F_PTR p_matrix, BOXFPOINTS_PTR p_out);