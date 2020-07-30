#include "RMatrix.h"

#include <string.h>

namespace _MathV2
{
	/// GENERIC MULTIPLICATIONS
	inline float* RMatrix_float_getAddresFromRaw(const float* p_mat, int p_columnSize, int p_column, int p_line)
	{
		return	(float*)(((char*)p_mat) + (sizeof(float) * p_columnSize * p_column) + (sizeof(float) * p_line));
	};

	inline float RMatrix_float_multiplication_line_column(const float* p_left, const float* p_right, const int leftColumnCount, const int leftLineCount,
		const int rightColumnCount, const int rightLineCount, int p_leftLineIndex, int p_rightColumnIndex)
	{
		float l_return = 0.0f;
		for (int i = 0; i < leftColumnCount; i++)
		{
			l_return += (*RMatrix_float_getAddresFromRaw(p_left, leftLineCount, i, p_leftLineIndex) * *RMatrix_float_getAddresFromRaw(p_right, rightLineCount, p_rightColumnIndex, i));
		}
		return l_return;
	};

	inline void RMatrix_multiplication_matrix_matrix(const float* p_left, const float* p_right, const int leftColumnCount, const int leftLineCount,
		const int rightColumnCount, const int rightLineCount, float* p_out)
	{
		for (int i = 0; i < leftLineCount; i++)
		{
			for (int j = 0; j < rightColumnCount; j++)
			{
				*RMatrix_float_getAddresFromRaw(p_out, leftLineCount, j, i) = RMatrix_float_multiplication_line_column((const float*)p_left, (const float*)p_right,
					leftColumnCount, leftLineCount, rightColumnCount, rightLineCount, i, j);
			}
		}
	}
	///


	void RMatrix_3x3_buildFromColumn(const float p_col1[3], const float p_col2[3], const float p_col3[3], float p_out[3][3])
	{
		memcpy(p_out, p_col1, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 3)), p_col2, sizeof(float) * 3);
		memcpy((float*)((char*)p_out + (sizeof(float) * 6)), p_col3, sizeof(float) * 3);
	};

	void RMatrix_4x4_mul_4x4(const float p_left[4][4], const float p_right[4][4], float p_out[4][4])
	{
		RMatrix_multiplication_matrix_matrix((const float*)p_left, (const float*)p_right, 4, 4, 4, 4, (float*)p_out);
	};

	void RMatrix_4x4_mul_4(const float p_left[4][4], const float p_right[4], float p_out[4])
	{
		RMatrix_multiplication_matrix_matrix((const float*)p_left, (const float*)p_right, 4, 4, 1, 4, (float*)p_out);
	};

	void RMatrix_3x3_mul_3x3(const float p_left[3][3], const float p_right[3][3], float p_out[3][3])
	{
		RMatrix_multiplication_matrix_matrix((const float*)p_left, (const float*)p_right, 3, 3, 3, 3, (float*)p_out);
	};
}