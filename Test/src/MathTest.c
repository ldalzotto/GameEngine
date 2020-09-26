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


/*
void Mat_Mul_M4F_M4F(const Matrix4f_PTR p_left, const Matrix4f_PTR p_right, Matrix4f_PTR p_out)
{
	p_out->_00 = (p_left->_00 * p_right->_00) + (p_left->_10 * p_right->_01) + (p_left->_20 * p_right->_02) + (p_left->_30 * p_right->_03);
	p_out->_01 = (p_left->_01 * p_right->_00) + (p_left->_11 * p_right->_01) + (p_left->_21 * p_right->_02) + (p_left->_31 * p_right->_03);
	p_out->_02 = (p_left->_02 * p_right->_00) + (p_left->_12 * p_right->_01) + (p_left->_22 * p_right->_02) + (p_left->_32 * p_right->_03);
	p_out->_03 = (p_left->_03 * p_right->_00) + (p_left->_13 * p_right->_01) + (p_left->_23 * p_right->_02) + (p_left->_33 * p_right->_03);

	p_out->_10 = (p_left->_00 * p_right->_10) + (p_left->_10 * p_right->_11) + (p_left->_20 * p_right->_12) + (p_left->_30 * p_right->_13);
	p_out->_11 = (p_left->_01 * p_right->_10) + (p_left->_11 * p_right->_11) + (p_left->_21 * p_right->_12) + (p_left->_31 * p_right->_13);
	p_out->_12 = (p_left->_02 * p_right->_10) + (p_left->_12 * p_right->_11) + (p_left->_22 * p_right->_12) + (p_left->_32 * p_right->_13);
	p_out->_13 = (p_left->_03 * p_right->_10) + (p_left->_13 * p_right->_11) + (p_left->_23 * p_right->_12) + (p_left->_33 * p_right->_13);

	p_out->_20 = (p_left->_00 * p_right->_20) + (p_left->_10 * p_right->_21) + (p_left->_20 * p_right->_22) + (p_left->_30 * p_right->_23);
	p_out->_21 = (p_left->_01 * p_right->_20) + (p_left->_11 * p_right->_21) + (p_left->_21 * p_right->_22) + (p_left->_31 * p_right->_23);
	p_out->_22 = (p_left->_02 * p_right->_20) + (p_left->_12 * p_right->_21) + (p_left->_22 * p_right->_22) + (p_left->_32 * p_right->_23);
	p_out->_23 = (p_left->_03 * p_right->_20) + (p_left->_13 * p_right->_21) + (p_left->_23 * p_right->_22) + (p_left->_33 * p_right->_23);

	p_out->_30 = (p_left->_00 * p_right->_30) + (p_left->_10 * p_right->_31) + (p_left->_20 * p_right->_32) + (p_left->_30 * p_right->_33);
	p_out->_31 = (p_left->_01 * p_right->_30) + (p_left->_11 * p_right->_31) + (p_left->_21 * p_right->_32) + (p_left->_31 * p_right->_33);
	p_out->_32 = (p_left->_02 * p_right->_30) + (p_left->_12 * p_right->_31) + (p_left->_22 * p_right->_32) + (p_left->_32 * p_right->_33);
	p_out->_33 = (p_left->_03 * p_right->_30) + (p_left->_13 * p_right->_31) + (p_left->_23 * p_right->_32) + (p_left->_33 * p_right->_33);
};
*/

void Mat_Mul_M4F_M4F_normal(const Matrix4f_PTR p_left, const Matrix4f_PTR p_right, Matrix4f_PTR p_out)
{
	p_out->_00 = (p_left->_00 * p_right->_00) + (p_left->_10 * p_right->_01) + (p_left->_20 * p_right->_02) + (p_left->_30 * p_right->_03);
	p_out->_01 = (p_left->_01 * p_right->_00) + (p_left->_11 * p_right->_01) + (p_left->_21 * p_right->_02) + (p_left->_31 * p_right->_03);
	p_out->_02 = (p_left->_02 * p_right->_00) + (p_left->_12 * p_right->_01) + (p_left->_22 * p_right->_02) + (p_left->_32 * p_right->_03);
	p_out->_03 = (p_left->_03 * p_right->_00) + (p_left->_13 * p_right->_01) + (p_left->_23 * p_right->_02) + (p_left->_33 * p_right->_03);

	p_out->_10 = (p_left->_00 * p_right->_10) + (p_left->_10 * p_right->_11) + (p_left->_20 * p_right->_12) + (p_left->_30 * p_right->_13);
	p_out->_11 = (p_left->_01 * p_right->_10) + (p_left->_11 * p_right->_11) + (p_left->_21 * p_right->_12) + (p_left->_31 * p_right->_13);
	p_out->_12 = (p_left->_02 * p_right->_10) + (p_left->_12 * p_right->_11) + (p_left->_22 * p_right->_12) + (p_left->_32 * p_right->_13);
	p_out->_13 = (p_left->_03 * p_right->_10) + (p_left->_13 * p_right->_11) + (p_left->_23 * p_right->_12) + (p_left->_33 * p_right->_13);
	
	p_out->_20 = (p_left->_00 * p_right->_20) + (p_left->_10 * p_right->_21) + (p_left->_20 * p_right->_22) + (p_left->_30 * p_right->_23);
	p_out->_21 = (p_left->_01 * p_right->_20) + (p_left->_11 * p_right->_21) + (p_left->_21 * p_right->_22) + (p_left->_31 * p_right->_23);
	p_out->_22 = (p_left->_02 * p_right->_20) + (p_left->_12 * p_right->_21) + (p_left->_22 * p_right->_22) + (p_left->_32 * p_right->_23);
	p_out->_23 = (p_left->_03 * p_right->_20) + (p_left->_13 * p_right->_21) + (p_left->_23 * p_right->_22) + (p_left->_33 * p_right->_23);
	
	p_out->_30 = (p_left->_00 * p_right->_30) + (p_left->_10 * p_right->_31) + (p_left->_20 * p_right->_32) + (p_left->_30 * p_right->_33);
	p_out->_31 = (p_left->_01 * p_right->_30) + (p_left->_11 * p_right->_31) + (p_left->_21 * p_right->_32) + (p_left->_31 * p_right->_33);
	p_out->_32 = (p_left->_02 * p_right->_30) + (p_left->_12 * p_right->_31) + (p_left->_22 * p_right->_32) + (p_left->_32 * p_right->_33);
	p_out->_33 = (p_left->_03 * p_right->_30) + (p_left->_13 * p_right->_31) + (p_left->_23 * p_right->_32) + (p_left->_33 * p_right->_33);
};

#include <intrin.h>

void Mat_Mul_M4F_M4F_128(const float* a, const float* b, float* r)
{
	__m128 a_line, b_line, r_line;
	for (int i = 0; i < 16; i += 4) {
		// unroll the first step of the loop to avoid having to initialize r_line to zero
		a_line = _mm_load_ps(a);         // a_line = vec4(column(a, 0))
		b_line = _mm_set_ps1(b[i]);      // b_line = vec4(b[i][0])
		r_line = _mm_mul_ps(a_line, b_line); // r_line = a_line * b_line
		for (int j = 1; j < 4; j++) {
			a_line = _mm_load_ps(&a[j * 4]); // a_line = vec4(column(a, j))
			b_line = _mm_set_ps1(b[i + j]);  // b_line = vec4(b[i][j])
										   // r_line += a_line * b_line
			r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
		}
		_mm_store_ps(&r[i], r_line);     // r[i] = r_line
	}

	// _mm_reduce_ps()

	// p_out->_01 = (p_left->_01 * p_right->_00) + (p_left->_11 * p_right->_01) + (p_left->_21 * p_right->_02) + (p_left->_31 * p_right->_03);
	// p_out->_02 = (p_left->_02 * p_right->_00) + (p_left->_12 * p_right->_01) + (p_left->_22 * p_right->_02) + (p_left->_32 * p_right->_03);
	// p_out->_03 = (p_left->_03 * p_right->_00) + (p_left->_13 * p_right->_01) + (p_left->_23 * p_right->_02) + (p_left->_33 * p_right->_03);
	
	// p_out->_10 = (p_left->_00 * p_right->_10) + (p_left->_10 * p_right->_11) + (p_left->_20 * p_right->_12) + (p_left->_30 * p_right->_13);
	// p_out->_11 = (p_left->_01 * p_right->_10) + (p_left->_11 * p_right->_11) + (p_left->_21 * p_right->_12) + (p_left->_31 * p_right->_13);
	// p_out->_12 = (p_left->_02 * p_right->_10) + (p_left->_12 * p_right->_11) + (p_left->_22 * p_right->_12) + (p_left->_32 * p_right->_13);
	// p_out->_13 = (p_left->_03 * p_right->_10) + (p_left->_13 * p_right->_11) + (p_left->_23 * p_right->_12) + (p_left->_33 * p_right->_13);
	// 
	// p_out->_20 = (p_left->_00 * p_right->_20) + (p_left->_10 * p_right->_21) + (p_left->_20 * p_right->_22) + (p_left->_30 * p_right->_23);
	// p_out->_21 = (p_left->_01 * p_right->_20) + (p_left->_11 * p_right->_21) + (p_left->_21 * p_right->_22) + (p_left->_31 * p_right->_23);
	// p_out->_22 = (p_left->_02 * p_right->_20) + (p_left->_12 * p_right->_21) + (p_left->_22 * p_right->_22) + (p_left->_32 * p_right->_23);
	// p_out->_23 = (p_left->_03 * p_right->_20) + (p_left->_13 * p_right->_21) + (p_left->_23 * p_right->_22) + (p_left->_33 * p_right->_23);
	// 
	// p_out->_30 = (p_left->_00 * p_right->_30) + (p_left->_10 * p_right->_31) + (p_left->_20 * p_right->_32) + (p_left->_30 * p_right->_33);
	// p_out->_31 = (p_left->_01 * p_right->_30) + (p_left->_11 * p_right->_31) + (p_left->_21 * p_right->_32) + (p_left->_31 * p_right->_33);
	// p_out->_32 = (p_left->_02 * p_right->_30) + (p_left->_12 * p_right->_31) + (p_left->_22 * p_right->_32) + (p_left->_32 * p_right->_33);
	// p_out->_33 = (p_left->_03 * p_right->_30) + (p_left->_13 * p_right->_31) + (p_left->_23 * p_right->_32) + (p_left->_33 * p_right->_33);
	
};

void Mat_Mul_M4F_128(const Matrix4f_PTR p_left, const Vector4f_PTR p_right, Vector4f_PTR p_out)
{
	// _mm_load_ps1()
	__m128 l_right = _mm_load_ps1(&p_right->Points[0]);
	__m128 l_leftColum = _mm_load_ps(&p_left->Points[0]);
	__m128 l_out = _mm_mul_ps(l_leftColum, l_right);
	for (short int i = 1; i < 4; i++)
	{
		l_right = _mm_load_ps1(&p_right->Points[i]);
		l_leftColum = _mm_load_ps(&p_left->Points[i * 4]);
		l_out = _mm_add_ps(l_out, _mm_mul_ps(l_leftColum, l_right));
		// l_left = _mm_load_ps(&p_left->Points[i * 4]);
	}
	_mm_store_ps(&p_out->Points[0], l_out);
	// Mat_Mul_MXxXf_MXxXf((const char*)p_left, (const char*)p_right, 4, 1, sizeof(Vector4f), sizeof(Vector4f), (char*)p_out);
};
#include <stdio.h>

void main()
{
	Matrix4f l_left, l_right, l_out;

	size_t l_time = Clock_currentTime_mics();
	for (size_t i = 0; i < 1000000; i++)
	{
		Mat_Mul_M4F_V4F(&l_left, (Vector4f_PTR)&l_right, (Vector4f_PTR)&l_out);
	}

	printf("%lld\n", Clock_currentTime_mics() - l_time);
	l_time = Clock_currentTime_mics();
	for (size_t i = 0; i < 1000000; i++)
	{
		Mat_Mul_M4F_128(&l_left, (Vector4f_PTR)&l_right, (Vector4f_PTR)&l_out);
	}
	printf("%lld\n", Clock_currentTime_mics() - l_time);

	printf("%lld\n", l_out._00);
}

#if 0
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

	// Mat_Mul_M4F_M4F()

}
#endif