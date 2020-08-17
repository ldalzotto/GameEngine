
extern "C"
{
#include "v2/_source/VectorC.h"
#include "v2/_source/QuaternionC.h"
#include "v2/_source/MatrixC.h"
}

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Quaternion/RQuaternion.h"

int main()
{
	MATRIX4F l_mat = { 10.0f, 1.0f, 2.0f, 3.0f , 4.0f , 9.0f , 11.0f , 12.0f , 5.0f , 6.0f , 7.0f , 8.0f , 13.0f , 14.0f , 15.0f , 16.0f };
	MATRIX4F l_out, l_out2;

	Mat_Inv_M4F(&l_mat, &l_out);
	_MathV2::RMatrix_4x4_inv((const float(*)[4])l_mat.Points, (float(*)[4])l_out2.Points);


	return 0;
}