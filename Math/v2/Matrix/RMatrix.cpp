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

	void RMatrix_multiplication_matrix_float(const float* p_left, const float p_right, const int leftColumnCount, const int leftLineCount, float* p_out)
	{
		for (int i = 0; i < leftLineCount; i++)
		{
			for (int j = 0; j < leftColumnCount; j++)
			{
				*RMatrix_float_getAddresFromRaw(p_out, leftLineCount, j, i) = *RMatrix_float_getAddresFromRaw(p_left, leftLineCount, j, i) * p_right;
			}
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
							l_mat3x3[l_columnCounter][l_rowCounter] = *RMatrix_float_getAddresFromRaw((const float*)m, 4, c, l);
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

	void RMatrix_4x4_mul_1(const float p_left[4][4], const float p_right, float p_out[4])
	{
		RMatrix_multiplication_matrix_float((const float*)p_left, p_right, 4, 4, (float*)p_out);
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

		RMatrix_multiplication_matrix_float((const float*)p_out, 1 / l_det, 4, 4, (float*)p_out);
	};

	void RMatrix_3x3_mul_3x3(const float p_left[3][3], const float p_right[3][3], float p_out[3][3])
	{
		RMatrix_multiplication_matrix_matrix((const float*)p_left, (const float*)p_right, 3, 3, 3, 3, (float*)p_out);
	};
}