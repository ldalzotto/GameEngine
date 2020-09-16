#include <stdlib.h>

#include "v2/_interface/MatrixC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/VectorC.h"
#include "v2/Math.h"
#include "v2/_interface/ColorC.h"
#include "DataStructures/ARRAY.h"
#include "Constants.h"
#include "Algorithm/Sort.h"
#include "Clock/Clock.h"

void main(int argc, char* argv[])
{
	srand((unsigned int)Clock_currentTime_mics());

	Matrix4f l_perspectiveMatrix;
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,	(800.0f / 600.0f), 0.1f, 50.0f, &l_perspectiveMatrix);

	Vector4f l_cameraRelativePos = { 0.0f, 10.0f, 20.0f, 1.0f };
	Vector4f l_clipPos;

	Mat_Mul_M4F_V4F_Homogeneous(&l_perspectiveMatrix, &l_cameraRelativePos, &l_clipPos);



	Frustum l_fr;
	Frustum_ExtractFromProjection(&l_perspectiveMatrix, &l_fr);
	
	Vector3f l_crossTest;
	Vec_Cross_3f(&Vector3f_FORWARD, &Vector3f_RIGHT, &l_crossTest);

	Color3f l_col = {0.0f, 0.5f, 2.0f};
	Color3c l_colc;
	Color_Convert_3F_3C(&l_col, &l_colc);

	int zd = 0;


	Array_Int l_arrInt; Arr_Alloc_Int(&l_arrInt, 0);
	for (size_t i = 0; i < 10; i++)
	{
		int l_r = rand();
		Arr_PushBackRealloc_Int(&l_arrInt, l_r);
	}

	Sort_Quick_Int_Decreasing(&l_arrInt);

}