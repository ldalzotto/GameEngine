#pragma once

namespace _MathV2
{
	
	void RMatrix_4x4_setColumn(const float p_mat[4][4], const int p_columnIndex, const float* p_col, const int p_col_lineCount);
// 	void RMatrix_4x4_copyMatrix3x3(const float p_mat[4][4], const int p_insertionColumnIndex, const int p_insertionLineIndex, const float p_insertedMatrix[3][3], float p_out[4][4]);
	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3]);

	void RMatrix_4x4_mul_4x4(const float p_left[4][4], const float p_right[4][4], float p_out[4][4]);
	void RMatrix_4x4_mul_4(const float p_left[4][4], const float p_right[4], float p_out[4]);
	void RMatrix_3x3_mul_3x3(const float p_left[3][3], const float p_right[3][3], float p_out[3][3]);

	void RMatrix_4x4_inv(const float p_matrix[4][4], float p_out[4][4]);
}