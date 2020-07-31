#pragma once

namespace _MathV2
{

	void RMatrix_4x4_setColumn(const float p_mat[4][4], const int p_columnIndex, const float* p_col, const int p_col_lineCount);
	void RMatrix_4x4_copyMatrix3x3(float p_destination[4][4], const float p_source[3][3], const short int p_destination_col_index, const short int p_destination_line_index);
	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3]);

	void RMatrix_4x4_mul_4x4(const float p_left[4][4], const float p_right[4][4], float p_out[4][4]);
	void RMatrix_4x4_mul_4(const float p_left[4][4], const float p_right[4], float p_out[4]);
	void RMatrix_3x3_mul_3x3(const float p_left[3][3], const float p_right[3][3], float p_out[3][3]);
	void RMatrix_3x3_mul_3(const float p_left[3][3], const float p_right[3], float p_out[3]);

	void RMatrix_4x4_inv(const float p_matrix[4][4], float p_out[4][4]);

	void RMatrix_4x4_buildTranslationMatrix(float p_matrix[4][4], const float p_translation[3]);
	void RMatrix_4x4_buildRotationMatrix(float p_matrix[4][4], const float p_axis[3][3]);
	void RMatrix_4x4_buildRotationMatrix(float p_matrix[4][4], const float p_right[3], const float p_up[3], const float p_forward[3]);
	void RMatrix_4x4_buildScaleMatrix(float p_matrix[4][4], const float p_scale[3]);

	void RMatrix_4x4_buildTRS(const float p_position[3], const  float p_quaternion[4], const  float p_scale[3], float out_TRS[4][4]);
	void RMatrix_4x4_buildTRS(const float p_position[3], const float p_right[3], const float p_up[3], const float p_forward[3], const  float p_scale[3], float out_TRS[4][4]);
	void RMatrix_4x4_getTranslation(const float p_trs[4][4], float out_translation[4]);
	void RMatrix_4x4_getScale(const float p_trs[4][4], float out_scale[4]);

	void RMatrix_4x4_perspective(const float p_fov, const float p_aspect, const float p_near, const float p_far, float p_out[4][4]);
	void RMatrix_4x4_clipSpaceMul(const float p_projectionmatrix[4][4], const float p_pos[4], float out_pos[4]);
}