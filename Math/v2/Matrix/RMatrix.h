#pragma once

namespace _MathV2
{
	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3]);

	void RMatrix_4x4_mul_4x4(const float p_left[4][4], const float p_right[4][4], float p_out[4][4]);
	void RMatrix_4x4_mul_4(const float p_left[4][4], const float p_right[4], float p_out[4]);
	void RMatrix_3x3_mul_3x3(const float p_left[3][3], const float p_right[3][3], float p_out[3][3]);
}