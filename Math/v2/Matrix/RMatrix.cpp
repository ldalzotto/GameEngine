#include "RMatrix.h"

#include "v2/Vector/RVector.h"
#include "v2/Quaternion/RQuaternion.h"
#include <string.h>
#include <math.h>

namespace _MathV2
{
	inline RMatrix RMatrix_build(const void* p_rawPointer, const short int p_columnCount, const short int p_lineCount)
	{
		return
			RMatrix{
			(char*)p_rawPointer,
			p_columnCount,
			p_lineCount
		};
	};

	/// GENERIC MULTIPLICATIONS
	template <typename T>
	inline float* RMatrix_float_getAddresFromRaw_v2(const RMatrix& p_matrix, int p_column, int p_line)
	{
		return	(float*)((p_matrix.Memory) + (sizeof(T) * p_matrix.LineCount * p_column) + (sizeof(T) * p_line));
	};

	template <typename LEFT_TYPE, typename RIGHT_TYPE, typename OUT_TYPE>
	inline void RMatrix_multiplication_matrix_scalar_v2(const RMatrix& p_left, const RIGHT_TYPE p_right, const RMatrix& p_out)
	{
		for (int i = 0; i < p_left.LineCount; i++)
		{
			for (int j = 0; j < p_left.ColumnCount; j++)
			{
				*RMatrix_float_getAddresFromRaw_v2<OUT_TYPE>(p_out, j, i) = *RMatrix_float_getAddresFromRaw_v2<LEFT_TYPE>(p_left, j, i) * p_right;
			}
		}
	};

	inline void* RMatrix_float_getAddresFromRaw_v3(const RMatrix& p_matrix, int p_column, int p_line, const size_t p_elementSize)
	{
		return	(void*)((p_matrix.Memory) + (p_elementSize * p_matrix.LineCount * p_column) + (p_elementSize * p_line));
	};

	inline float RMatrix_multiplication_line_column_v3(const RMatrix& p_left, const size_t p_left_elementSize, const RMatrix& p_right, const size_t p_right_elementSize, int p_leftLineIndex, int p_rightColumnIndex)
	{
		float l_return = 0.0f;
		for (short int i = 0; i < p_left.ColumnCount; i++)
		{
			l_return += (*(float*)RMatrix_float_getAddresFromRaw_v3(p_left, i, p_leftLineIndex, p_left_elementSize) * *(float*)RMatrix_float_getAddresFromRaw_v3(p_right, p_rightColumnIndex, i, p_right_elementSize));
		}
		return l_return;
	};

	inline void RMatrix_multiplication_matrix_matrix_v3(const RMatrix& p_left, const size_t p_left_elementSize, const RMatrix& p_right, const size_t p_right_elementSize, RMatrix& p_out)
	{
		for (int i = 0; i < p_left.LineCount; i++)
		{
			for (int j = 0; j < p_right.ColumnCount; j++)
			{
				*(float*)RMatrix_float_getAddresFromRaw_v3(p_out, j, i, p_left_elementSize) = RMatrix_multiplication_line_column_v3(p_left, p_left_elementSize, p_right, p_right_elementSize, i, j);
			}
		}
	}


	template <typename DESTINATION_TYPE>
	inline void RMatrix_copy(const RMatrix& p_destination, const RMatrix& p_source, const short int p_destination_col_index, const short int p_destination_line_index)
	{
		for (short int j = 0; j < p_source.ColumnCount; j++)
		{
			memcpy(RMatrix_float_getAddresFromRaw_v2<DESTINATION_TYPE>(p_destination, j + p_destination_col_index, p_destination_line_index),
				RMatrix_float_getAddresFromRaw_v2<DESTINATION_TYPE>(p_source, j, 0), sizeof(DESTINATION_TYPE) * p_source.LineCount);
		}
	};


	float RMatrix_3x3det(const float m[4][4], const int p_col, const int p_line)
	{
		float l_mat3x3[3][3]{};
		{
			int l_columnCounter = 0;
			int l_rowCounter = 0;

			for (int c = 0; c < 4; c++)
			{
				if (c != p_col)
				{
					for (int l = 0; l < 4; l++)
					{
						if (l != p_line)
						{
							l_mat3x3[l_columnCounter][l_rowCounter] = *RMatrix_float_getAddresFromRaw_v2<float>(RMatrix{ (char*)m , 4, 4 }, c, l);
							l_rowCounter += 1;
						}
					}
					l_rowCounter = 0;
					l_columnCounter += 1;
				}
			}
		}

		return
			(l_mat3x3[0][0] * ((l_mat3x3[1][1] * l_mat3x3[2][2]) - (l_mat3x3[1][2] * l_mat3x3[2][1]))) +
			(l_mat3x3[1][0] * ((l_mat3x3[2][1] * l_mat3x3[0][2]) - (l_mat3x3[2][2] * l_mat3x3[0][1]))) +
			(l_mat3x3[2][0] * ((l_mat3x3[0][1] * l_mat3x3[1][2]) - (l_mat3x3[0][2] * l_mat3x3[1][1])))
			;
	}
	///

	void RMatrix_4x4_copyMatrix3x3(float p_destination[4][4], const float p_source[3][3], const short int p_destination_col_index, const short int p_destination_line_index)
	{
		RMatrix_copy<float>(RMatrix_build(p_destination, 4, 4), RMatrix_build(p_source, 3, 3), p_destination_col_index, p_destination_line_index);
	};

	void RMatrix_4x4_setColumn(const float p_mat[4][4], const int p_columnIndex, const float* p_col, const int p_col_lineCount)
	{
		RMatrix_copy<float>(RMatrix_build((const float*)p_mat, 4, 4), RMatrix_build((const float*)p_col, 1, p_col_lineCount), p_columnIndex, 0);
	};

	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3])
	{
		memcpy(p_out, p_col1, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 3)), p_col2, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 6)), p_col3, sizeof(float) * 3);
	};

	void RMatrix_4x4_buildFromColumn(const float p_col1[4], const float p_col2[4], const float p_col3[4], const float p_col4[4], float p_out[4][4])
	{
		memcpy(p_out, p_col1, sizeof(float) * 4);
		memcpy((float*)((char*)p_out + (sizeof(float) * 4)), p_col2, sizeof(float) * 4);
		memcpy((float*)((char*)p_out + (sizeof(float) * 8)), p_col3, sizeof(float) * 4);
		memcpy((float*)((char*)p_out + (sizeof(float) * 12)), p_col4, sizeof(float) * 4);
	};

	void RMatrix_mul_specification(
		const float* p_left, const  short int p_left_columnNumber, const  short int p_left_lineNumber,
		const float* p_right, const  short int p_right_columnNumber, const  short int p_right_lineNumber, float* p_out)
	{
		RMatrix_multiplication_matrix_matrix_v3(
			RMatrix_build(p_left, p_left_columnNumber, p_left_lineNumber), sizeof(float),
			RMatrix_build(p_right, p_right_columnNumber, p_right_lineNumber), sizeof(float),
			RMatrix_build(p_out, p_right_columnNumber, p_left_lineNumber));
	};

	void RMatrix_4x4_mul_1(const float p_left[4][4], const float p_right, float p_out[4])
	{
		RMatrix_multiplication_matrix_scalar_v2<float, float, float>(RMatrix_build((const float*)p_left, 4, 4), p_right, RMatrix_build((const float*)p_out, 4, 4));
	};

	void RMatrix_4x4_inv(const float p_matrix[4][4], float p_out[4][4])
	{
		float l_det = (p_matrix[0][0] * RMatrix_3x3det(p_matrix, 0, 0)) - (p_matrix[0][1] * RMatrix_3x3det(p_matrix, 0, 1)) + (p_matrix[0][2] * RMatrix_3x3det(p_matrix, 0, 2)) - (p_matrix[0][3] * RMatrix_3x3det(p_matrix, 0, 3));

		{
			p_out[0][0] = RMatrix_3x3det(p_matrix, 0, 0);
			p_out[0][1] = -RMatrix_3x3det(p_matrix, 1, 0);
			p_out[0][2] = RMatrix_3x3det(p_matrix, 2, 0);
			p_out[0][3] = -RMatrix_3x3det(p_matrix, 3, 0);
			p_out[1][0] = -RMatrix_3x3det(p_matrix, 0, 1);
			p_out[1][1] = RMatrix_3x3det(p_matrix, 1, 1);
			p_out[1][2] = -RMatrix_3x3det(p_matrix, 2, 1);
			p_out[1][3] = RMatrix_3x3det(p_matrix, 3, 1);
			p_out[2][0] = RMatrix_3x3det(p_matrix, 0, 2);
			p_out[2][1] = -RMatrix_3x3det(p_matrix, 1, 2);
			p_out[2][2] = RMatrix_3x3det(p_matrix, 2, 2);
			p_out[2][3] = -RMatrix_3x3det(p_matrix, 3, 2);
			p_out[3][0] = -RMatrix_3x3det(p_matrix, 0, 3);
			p_out[3][1] = RMatrix_3x3det(p_matrix, 1, 3);
			p_out[3][2] = -RMatrix_3x3det(p_matrix, 2, 3);
			p_out[3][3] = RMatrix_3x3det(p_matrix, 3, 3);
		}

		RMatrix_multiplication_matrix_scalar_v2<float, float, float>(RMatrix_build((const float*)p_out, 4, 4), 1 / l_det, RMatrix_build((const float*)p_out, 4, 4));
	};

	void RMatrix_4x4_buildTranslationMatrix(float p_matrix[4][4], const float p_translation[3])
	{
		RMatrix_4x4_setColumn(p_matrix, 3, p_translation, 3);
	};

	void RMatrix_4x4_buildRotationMatrix(float p_matrix[4][4], const float p_axis[3][3])
	{
		RMatrix_4x4_copyMatrix3x3(p_matrix, p_axis, 0, 0);
	};

	void RMatrix_4x4_buildRotationMatrix(float p_matrix[4][4], const float p_right[3], const float p_up[3], const float p_forward[3])
	{
		RMatrix_4x4_setColumn(p_matrix, 0, p_up, 3);
		RMatrix_4x4_setColumn(p_matrix, 1, p_forward, 3);
		RMatrix_4x4_setColumn(p_matrix, 2, p_forward, 3);
	};

	void RMatrix_4x4_buildScaleMatrix(float p_matrix[4][4], const float p_scale[3])
	{
		RVector_3_mul(p_matrix[0], p_scale[0], p_matrix[0]);
		RVector_3_mul(p_matrix[1], p_scale[1], p_matrix[1]);
		RVector_3_mul(p_matrix[2], p_scale[2], p_matrix[2]);
	};

	void RMatrix_4x4_buildTRS(const float p_position[3], const float p_quaternion[4], const  float p_scale[3], float out_TRS[4][4])
	{
		out_TRS[0][3] = 0.0f;
		out_TRS[1][3] = 0.0f;
		out_TRS[2][3] = 0.0f;
		out_TRS[3][3] = 1.0f;

		RMatrix_4x4_buildTranslationMatrix(out_TRS, p_position);
		float l_axis[3][3];
		RQuaternion_extractAxis(p_quaternion, l_axis);
		RMatrix_4x4_buildRotationMatrix(out_TRS, l_axis);
		RMatrix_4x4_buildScaleMatrix(out_TRS, p_scale);
	};

	void RMatrix_4x4_buildTRS(const float p_position[3], const float p_axis[3][3], const  float p_scale[3], float out_TRS[4][4])
	{
		out_TRS[0][3] = 0.0f;
		out_TRS[1][3] = 0.0f;
		out_TRS[2][3] = 0.0f;
		out_TRS[3][3] = 1.0f;

		RMatrix_4x4_buildTranslationMatrix(out_TRS, p_position);
		RMatrix_4x4_buildRotationMatrix(out_TRS, p_axis);
		RMatrix_4x4_buildScaleMatrix(out_TRS, p_scale);
	};

	void RMatrix_4x4_getTranslation(const float p_trs[4][4], float out_translation[4])
	{
		memcpy(out_translation, p_trs[3], sizeof(float) * 4);
	};

	void RMatrix_4x4_getScale(const float p_trs[4][4], float out_scale[4])
	{
		float l_c0[4], l_c1[4], l_c2[4];
		memcpy(l_c0, p_trs[0], sizeof(float) * 4);
		memcpy(l_c0, p_trs[1], sizeof(float) * 4);
		memcpy(l_c0, p_trs[2], sizeof(float) * 4);

		out_scale[0] = RVector_length_specialization(l_c0, 4);
		out_scale[1] = RVector_length_specialization(l_c1, 4);
		out_scale[2] = RVector_length_specialization(l_c2, 4);
		out_scale[3] = 0.0f;
	};


	void RMatrix_4x4_perspective(const float p_fov, const float p_aspect, const float p_near, const float p_far, float p_out[4][4])
	{
		float l_halfTan = tanf(p_fov / 2.0f);

		p_out[0][0] = 1.0f / (p_aspect * l_halfTan);
		p_out[0][1] = 0.0f;
		p_out[0][2] = 0.0f;
		p_out[0][3] = 0.0f;

		p_out[1][0] = 0.0f;
		p_out[1][1] = 1.0f / l_halfTan;
		p_out[1][2] = 0.0f;
		p_out[1][3] = 0.0f;

#if !RENDER_V2
		p_out[2][0] = 0.0f;
		p_out[2][1] = 0.0f;
		p_out[2][2] = -(p_far + p_near) / (p_far - p_near);
		p_out[2][3] = -1.0f;

		p_out[3][0] = 0.0f;
		p_out[3][1] = 0.0f;
		p_out[3][2] = (-2.0f * p_far * p_near) / (p_far - p_near);
		p_out[3][3] = 0.0f;
#else
		p_out[2][0] = 0.0f;
		p_out[2][1] = 0.0f;
		p_out[2][2] = (p_far - p_near) / (p_far + p_near) ;
		p_out[2][3] = 1.0f;

		p_out[3][0] = 0.0f;
		p_out[3][1] = 0.0f;
		p_out[3][2] = (-2.0f * p_far * p_near) / (p_far - p_near);
		p_out[3][3] = 0.0f;
#endif
	};

	void RMatrix_3x3_lookAt_rotation(const float p_origin[3], const float p_target[3], const float p_up[3], float out_rotationMatrix[3][3])
	{
		//out_rotationMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		float l_forward[3];
		{
			RVector_min_specification(p_target, p_origin, l_forward, 3);
			RVector_normalize_specialization(l_forward, 3, l_forward);
#if !RENDER_V2
			RVector_3_mul(l_forward, -1.0f, l_forward);
#endif
		}
		float l_right[3];
		{
			RVector_3_cross(l_forward, p_up, l_right);
			RVector_normalize_specialization(l_right, 3, l_right);
		}
		float l_up[3];
		{
			RVector_3_cross(l_right, l_forward, l_up);
			RVector_normalize_specialization(l_up, 3, l_up);
		}

		RMatrix_3x3_buildFromColumn(l_right, l_up, l_forward, out_rotationMatrix);
	};

	void RMatrix_4x4_clipSpaceMul(const float p_projectionmatrix[4][4], const float p_pos[4], float out_pos[4])
	{
		RMatrix_mul_specification((const float*)p_projectionmatrix, 4, 4, p_pos, 1, 4, out_pos);
		// RMatrix_4x4_mul_4(p_projectionmatrix, p_pos, out_pos);
		RVector_4_mul(out_pos, 1 / out_pos[3], out_pos);
	};
}