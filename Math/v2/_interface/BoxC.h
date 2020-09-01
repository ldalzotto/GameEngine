#pragma once

#include "VectorStructuresC_def.h"
#include "BoxC_def.h"
#include "MatrixC_def.h"

typedef struct BoxFPoints_TYP
{
	union 
	{
		struct
		{
			Vector3f L_D_F;
			Vector3f L_U_F;

			Vector3f R_D_F;
			Vector3f R_U_F;

			Vector3f L_D_B;
			Vector3f L_U_B;

			Vector3f R_D_B;
			Vector3f R_U_B;

			Vector3f Center;
		};

		Vector3f Points[9];
	};
	
}BoxFPoints, * BoxFPoints_PTR;

//TODO using an iterator to avoid copying data
void Box_Build_F(BoxF_PTR p_box, Array_Vector3f_PTR p_points);
void Box_ExtractPoints_F(const BoxF_PTR p_box, BoxFPoints_PTR p_out);
void Box_ExtractMinMax_F(const BoxF_PTR p_box, Vector3f_PTR out_min, Vector3f_PTR out_max);
void BoxPoints_Mul_F_M4F(const BoxFPoints_PTR p_boxPoints, const Matrix4f_PTR p_matrix, BoxFPoints_PTR p_out);