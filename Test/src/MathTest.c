#include "v2/_interface/MatrixC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/VectorC.h"
#include "v2/Math.h"
#include "Constants.h"

void main()
{
	Matrix4f l_perspectiveMatrix;
	Mat_Perspective_M4F(45.0f * DEG_TO_RAD,	(800.0f / 600.0f), 0.1f, 50.0f, &l_perspectiveMatrix);

	Vector4f l_cameraRelativePos = { 0.0f, 10.0f, 20.0f, 1.0f };
	Vector4f l_clipPos;

	Mat_Mul_M4F_V4F_Homogeneous(&l_perspectiveMatrix, &l_cameraRelativePos, &l_clipPos);



	Frustum l_fr;
	Frustum_ExtractFromProjection(&l_perspectiveMatrix, &l_fr);
	
	Vector3f l_crossTest;
	Vec_Cross_3f(&Vector3f_FORWARD, &Vector3f_RIGHT, &l_crossTest);

	
	int zd = 0;

}