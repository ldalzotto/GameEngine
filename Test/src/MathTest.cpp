
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
//	MATRIX4F l_mat = { 10.0f, 1.0f, 2.0f, 3.0f , 4.0f , 9.0f , 11.0f , 12.0f , 5.0f , 6.0f , 7.0f , 8.0f , 13.0f , 14.0f , 15.0f , 16.0f };
	MATRIX4F l_out, l_out2;
	VECTOR3F l_position = { 5.0f, 9.0f, -2.0f };
	QUATERNION4F l_quat = { 0.46f, 0.19f, 0.3333f, 0.1f };
	VECTOR3F l_scale = {2.0f, 3.05f, -1.5f};
	float lzd[4][4];
	_MathV2::RMatrix_4x4_buildTRS(l_position.Points, l_quat.Points.Points, l_scale.Points, lzd);
	// *p_out = Matrix4x4f_Identity;
	Mat_TRS_Quat_M4F(&l_position, &l_quat, &l_scale, &l_out2);

	return 0;
}