#include "MatrixMath.h"

#include <math.h>

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Quaternion/QuaternionMath.h"

namespace _GameEngine::_Math
{
	float Matrixf4x4_get_unsafe(Matrix4x4f* p_matrix, int p_col, int p_line)
	{
		return *(float*)(((char*)&p_matrix->_00) + (sizeof(float) * ((4 * p_col) + p_line)));
	};

	void Matrix4x4f_c0(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_00; p_out->y = p_matrix->_01; p_out->z = p_matrix->_02; p_out->w = p_matrix->_03; };
	void Matrix4x4f_c1(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_10; p_out->y = p_matrix->_11; p_out->z = p_matrix->_12; p_out->w = p_matrix->_13; };
	void Matrix4x4f_c2(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_20; p_out->y = p_matrix->_21; p_out->z = p_matrix->_22; p_out->w = p_matrix->_23; };
	void Matrix4x4f_c3(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_30; p_out->y = p_matrix->_31; p_out->z = p_matrix->_32; p_out->w = p_matrix->_33; };

	void Matrix4x4f_set_c0(Matrix4x4f* p_matrix, Vector4f* p_col) { p_matrix->_00 = p_col->x; p_matrix->_01 = p_col->y; p_matrix->_02 = p_col->z; p_matrix->_03 = p_col->w; };
	void Matrix4x4f_set_c1(Matrix4x4f* p_matrix, Vector4f* p_col) { p_matrix->_10 = p_col->x; p_matrix->_11 = p_col->y; p_matrix->_12 = p_col->z; p_matrix->_13 = p_col->w; };
	void Matrix4x4f_set_c2(Matrix4x4f* p_matrix, Vector4f* p_col) { p_matrix->_20 = p_col->x; p_matrix->_21 = p_col->y; p_matrix->_22 = p_col->z; p_matrix->_23 = p_col->w; };
	void Matrix4x4f_set_c3(Matrix4x4f* p_matrix, Vector4f* p_col) { p_matrix->_30 = p_col->x; p_matrix->_31 = p_col->y; p_matrix->_32 = p_col->z; p_matrix->_33 = p_col->w; };

	void Matrix4x4f_set_c0(Matrix4x4f* p_matrix, Vector3f* p_col) { p_matrix->_00 = p_col->x; p_matrix->_01 = p_col->y; p_matrix->_02 = p_col->z; };
	void Matrix4x4f_set_c1(Matrix4x4f* p_matrix, Vector3f* p_col) { p_matrix->_10 = p_col->x; p_matrix->_11 = p_col->y; p_matrix->_12 = p_col->z; };
	void Matrix4x4f_set_c2(Matrix4x4f* p_matrix, Vector3f* p_col) { p_matrix->_20 = p_col->x; p_matrix->_21 = p_col->y; p_matrix->_22 = p_col->z; };
	void Matrix4x4f_set_c3(Matrix4x4f* p_matrix, Vector3f* p_col) { p_matrix->_30 = p_col->x; p_matrix->_31 = p_col->y; p_matrix->_32 = p_col->z; };

	void Matrix3x3f_c0(Matrix3x3f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_00; p_out->y = p_matrix->_01; p_out->z = p_matrix->_02; };
	void Matrix3x3f_c1(Matrix3x3f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_10; p_out->y = p_matrix->_11; p_out->z = p_matrix->_12; };
	void Matrix3x3f_c2(Matrix3x3f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_20; p_out->y = p_matrix->_21; p_out->z = p_matrix->_22; };

	void Matrix4x4f_right(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_00; p_out->y = p_matrix->_01; p_out->z = p_matrix->_02; p_out->w = p_matrix->_03; };
	void Matrix4x4f_up(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_10; p_out->y = p_matrix->_11; p_out->z = p_matrix->_12; p_out->w = p_matrix->_13; };
	void Matrix4x4f_forward(Matrix4x4f* p_matrix, Vector4f* p_out) { p_out->x = p_matrix->_20; p_out->y = p_matrix->_21; p_out->z = p_matrix->_22; p_out->w = p_matrix->_23; };

	void Matrix4x4f_right(Matrix4x4f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_00; p_out->y = p_matrix->_01; p_out->z = p_matrix->_02; };
	void Matrix4x4f_up(Matrix4x4f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_10; p_out->y = p_matrix->_11; p_out->z = p_matrix->_12; };
	void Matrix4x4f_forward(Matrix4x4f* p_matrix, Vector3f* p_out) { p_out->x = p_matrix->_20; p_out->y = p_matrix->_21; p_out->z = p_matrix->_22; };

	void Matrixf4x4_build(Matrix3x3f* p_matrix, Matrix4x4f* p_out)
	{
		p_out->_00 = p_matrix->_00;
		p_out->_01 = p_matrix->_01;
		p_out->_02 = p_matrix->_02;

		p_out->_10 = p_matrix->_20;
		p_out->_11 = p_matrix->_21;
		p_out->_12 = p_matrix->_22;

		p_out->_20 = p_matrix->_20;
		p_out->_21 = p_matrix->_21;
		p_out->_22 = p_matrix->_22;

		p_out->_03 = 0.0f;
		p_out->_13 = 0.0f;
		p_out->_23 = 0.0f;
		p_out->_30 = 0.0f;
		p_out->_31 = 0.0f;
		p_out->_32 = 0.0f;
		p_out->_33 = 1.0f;
	};

	void Matrif4x4_buildTRS(Vector3f* p_position, Quaternionf* p_quaternion, Vector3f* p_scale, Matrix4x4f* out_TRS)
	{
		out_TRS->_03 = 0.0f;
		out_TRS->_13 = 0.0f;
		out_TRS->_23 = 0.0f;
		out_TRS->_33 = 1.0f;

		Matrixf4x4_buildTranslationMatrix(p_position, out_TRS);

		Vector3f l_right, l_up, l_forward;
		Quaternion_extractAxis(p_quaternion, &l_right, &l_up, &l_forward);

		Matrixf4x4_buildRotationMatrixV2(&l_right, &l_up, &l_forward, out_TRS);
		Matrixf4x4_buildScaleMatrix(p_scale, out_TRS);
	};


	void Matrif4x4_buildTRSV2(Vector3f* p_position, Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Vector3f* p_scale, Matrix4x4f* out_TRS)
	{
		out_TRS->_03 = 0.0f;
		out_TRS->_13 = 0.0f;
		out_TRS->_23 = 0.0f;
		out_TRS->_33 = 1.0f;

		Matrixf4x4_buildTranslationMatrix(p_position, out_TRS);
		Matrixf4x4_buildRotationMatrixV2(p_right, p_up, p_forward, out_TRS);
		Matrixf4x4_buildScaleMatrix(p_scale, out_TRS);
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, float p_value, Matrix4x4f* out)
	{
		out->_00 = p_matrix->_00 * p_value;
		out->_01 = p_matrix->_01 * p_value;
		out->_02 = p_matrix->_02 * p_value;
		out->_03 = p_matrix->_03 * p_value;

		out->_10 = p_matrix->_10 * p_value;
		out->_11 = p_matrix->_11 * p_value;
		out->_12 = p_matrix->_12 * p_value;
		out->_13 = p_matrix->_13 * p_value;

		out->_20 = p_matrix->_20 * p_value;
		out->_21 = p_matrix->_21 * p_value;
		out->_22 = p_matrix->_22 * p_value;
		out->_23 = p_matrix->_23 * p_value;

		out->_30 = p_matrix->_30 * p_value;
		out->_31 = p_matrix->_31 * p_value;
		out->_32 = p_matrix->_32 * p_value;
		out->_33 = p_matrix->_33 * p_value;
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector4f* p_vector, Vector4f* out)
	{
		out->x = (p_matrix->_00 * p_vector->x) + (p_matrix->_10 * p_vector->y) + (p_matrix->_20 * p_vector->z) + (p_matrix->_30 * p_vector->w);
		out->y = (p_matrix->_01 * p_vector->x) + (p_matrix->_11 * p_vector->y) + (p_matrix->_21 * p_vector->z) + (p_matrix->_31 * p_vector->w);
		out->z = (p_matrix->_02 * p_vector->x) + (p_matrix->_12 * p_vector->y) + (p_matrix->_22 * p_vector->z) + (p_matrix->_32 * p_vector->w);
		out->w = (p_matrix->_03 * p_vector->x) + (p_matrix->_13 * p_vector->y) + (p_matrix->_23 * p_vector->z) + (p_matrix->_33 * p_vector->w);
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector3f* p_vector, Vector3f* out)
	{
		out->x = (p_matrix->_00 * p_vector->x) + (p_matrix->_10 * p_vector->y) + (p_matrix->_20 * p_vector->z);
		out->y = (p_matrix->_01 * p_vector->x) + (p_matrix->_11 * p_vector->y) + (p_matrix->_21 * p_vector->z);
		out->z = (p_matrix->_02 * p_vector->x) + (p_matrix->_12 * p_vector->y) + (p_matrix->_22 * p_vector->z);
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Matrix4x4f* p_other, Matrix4x4f* out)
	{
		out->_00 = (p_matrix->_00 * p_other->_00) + (p_matrix->_10 * p_other->_01) + (p_matrix->_20 * p_other->_02) + (p_matrix->_30 * p_other->_03);
		out->_01 = (p_matrix->_01 * p_other->_00) + (p_matrix->_11 * p_other->_01) + (p_matrix->_21 * p_other->_02) + (p_matrix->_31 * p_other->_03);
		out->_02 = (p_matrix->_02 * p_other->_00) + (p_matrix->_12 * p_other->_01) + (p_matrix->_22 * p_other->_02) + (p_matrix->_32 * p_other->_03);
		out->_03 = (p_matrix->_03 * p_other->_00) + (p_matrix->_13 * p_other->_01) + (p_matrix->_23 * p_other->_02) + (p_matrix->_33 * p_other->_03);

		out->_10 = (p_matrix->_00 * p_other->_10) + (p_matrix->_10 * p_other->_11) + (p_matrix->_20 * p_other->_12) + (p_matrix->_30 * p_other->_13);
		out->_11 = (p_matrix->_01 * p_other->_10) + (p_matrix->_11 * p_other->_11) + (p_matrix->_21 * p_other->_12) + (p_matrix->_31 * p_other->_13);
		out->_12 = (p_matrix->_02 * p_other->_10) + (p_matrix->_12 * p_other->_11) + (p_matrix->_22 * p_other->_12) + (p_matrix->_32 * p_other->_13);
		out->_13 = (p_matrix->_03 * p_other->_10) + (p_matrix->_13 * p_other->_11) + (p_matrix->_23 * p_other->_12) + (p_matrix->_33 * p_other->_13);

		out->_20 = (p_matrix->_00 * p_other->_20) + (p_matrix->_10 * p_other->_21) + (p_matrix->_20 * p_other->_22) + (p_matrix->_30 * p_other->_23);
		out->_21 = (p_matrix->_01 * p_other->_20) + (p_matrix->_11 * p_other->_21) + (p_matrix->_21 * p_other->_22) + (p_matrix->_31 * p_other->_23);
		out->_22 = (p_matrix->_02 * p_other->_20) + (p_matrix->_12 * p_other->_21) + (p_matrix->_22 * p_other->_22) + (p_matrix->_32 * p_other->_23);
		out->_23 = (p_matrix->_03 * p_other->_20) + (p_matrix->_13 * p_other->_21) + (p_matrix->_23 * p_other->_22) + (p_matrix->_33 * p_other->_23);

		out->_30 = (p_matrix->_00 * p_other->_30) + (p_matrix->_10 * p_other->_31) + (p_matrix->_20 * p_other->_32) + (p_matrix->_30 * p_other->_33);
		out->_31 = (p_matrix->_01 * p_other->_30) + (p_matrix->_11 * p_other->_31) + (p_matrix->_21 * p_other->_32) + (p_matrix->_31 * p_other->_33);
		out->_32 = (p_matrix->_02 * p_other->_30) + (p_matrix->_12 * p_other->_31) + (p_matrix->_22 * p_other->_32) + (p_matrix->_32 * p_other->_33);
		out->_33 = (p_matrix->_03 * p_other->_30) + (p_matrix->_13 * p_other->_31) + (p_matrix->_23 * p_other->_32) + (p_matrix->_33 * p_other->_33);
	};

	float matrixf4x4_3x3det(Matrix4x4f* m, int p_col, int p_line)
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
							l_mat3x3[l_columnCounter][l_rowCounter] = Matrixf4x4_get_unsafe(m, c, l);
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

	// For an example, see https://semath.info/src/inverse-cofactor-ex4.html
	void Matrixf4x4_inv(Matrix4x4f* m, Matrix4x4f* p_out)
	{
		float l_det = (m->_00 * matrixf4x4_3x3det(m, 0, 0)) - (m->_01 * matrixf4x4_3x3det(m, 0, 1)) + (m->_02 * matrixf4x4_3x3det(m, 0, 2)) - (m->_03 * matrixf4x4_3x3det(m, 0, 3));

		{
			p_out->_00 = matrixf4x4_3x3det(m, 0, 0);
			p_out->_01 = -matrixf4x4_3x3det(m, 1, 0);
			p_out->_02 = matrixf4x4_3x3det(m, 2, 0);
			p_out->_03 = -matrixf4x4_3x3det(m, 3, 0);
			p_out->_10 = -matrixf4x4_3x3det(m, 0, 1);
			p_out->_11 = matrixf4x4_3x3det(m, 1, 1);
			p_out->_12 = -matrixf4x4_3x3det(m, 2, 1);
			p_out->_13 = matrixf4x4_3x3det(m, 3, 1);
			p_out->_20 = matrixf4x4_3x3det(m, 0, 2);
			p_out->_21 = -matrixf4x4_3x3det(m, 1, 2);
			p_out->_22 = matrixf4x4_3x3det(m, 2, 2);
			p_out->_23 = -matrixf4x4_3x3det(m, 3, 2);
			p_out->_30 = -matrixf4x4_3x3det(m, 0, 3);
			p_out->_31 = matrixf4x4_3x3det(m, 1, 3);
			p_out->_32 = -matrixf4x4_3x3det(m, 2, 3);
			p_out->_33 = matrixf4x4_3x3det(m, 3, 3);
		}

		Matrixf4x4_mul(p_out, 1 / l_det, p_out);
	};

	void Matrixf4x4_buildTranslationMatrix(Vector3f* p_translation, Matrix4x4f* p_out)
	{
		Matrix4x4f_set_c3(p_out, p_translation);
	}

	void Matrixf4x4_buildRotationMatrixV2(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Matrix4x4f* p_out)
	{
		Matrix4x4f_set_c0(p_out, p_right);
		Matrix4x4f_set_c1(p_out, p_up);
		Matrix4x4f_set_c2(p_out, p_forward);
	};

	void Matrixf4x4_buildScaleMatrix(Vector3f* p_scale, Matrix4x4f* p_out)
	{
		p_out->_00 *= p_scale->x;
		p_out->_01 *= p_scale->x;
		p_out->_02 *= p_scale->x;

		p_out->_10 *= p_scale->y;
		p_out->_11 *= p_scale->y;
		p_out->_12 *= p_scale->y;

		p_out->_20 *= p_scale->z;
		p_out->_21 *= p_scale->z;
		p_out->_22 *= p_scale->z;
	};

	void Matrixf4x4_extractTranslation(Matrix4x4f* p_mat, Vector4f* out_translation)
	{
		Matrix4x4f_c3(p_mat, out_translation);
	};

	void Matrixf4x4_extractScale(Matrix4x4f* p_mat, Vector4f* out_scale)
	{
		Vector4f l_c0, l_c1, l_c2;
		Matrix4x4f_c0(p_mat, &l_c0);
		Matrix4x4f_c1(p_mat, &l_c1);
		Matrix4x4f_c2(p_mat, &l_c2);
		
		out_scale->x = Vector4f_length(&l_c0);
		out_scale->y = Vector4f_length(&l_c1);
		out_scale->z = Vector4f_length(&l_c2);
		out_scale->w = 0.0f;
	};

	void Matrixf4x4_lookAtRotation(Vector3f* p_origin, Vector3f* p_target, Vector3f* p_up, Matrix4x4f* p_out)
	{
		Vector3f l_forward;
		{
			Vector3f_min(p_target, p_origin, &l_forward);
			Vector3f_normalize(&l_forward);
		}

		Vector3f l_right;
		{
			Vector3f_cross(&l_forward, p_up, &l_right);
			Vector3f_normalize(&l_right);
		}

		Vector3f l_up;
		{
			Vector3f_cross(&l_right, &l_forward, &l_up);
			Vector3f_normalize(&l_up);
		}

		Vector3f l_scale{ 1.0f, 1.0f, 1.0f };
		Matrif4x4_buildTRSV2(p_origin, &l_right, &l_up, &l_forward, &l_scale, p_out);
	};


	void Matrixf4x4_lookAt(Vector3f* p_origin, Vector3f* p_target, Vector3f* p_up, Matrix4x4f* p_out)
	{
		Vector3f l_forward;
		{
			Vector3f_min(p_target, p_origin, &l_forward);
			Vector3f_normalize(&l_forward);
			Vector3f_mul(&l_forward, -1.0f, &l_forward);
		}

		Vector3f l_right;
		{
			Vector3f_cross(&l_forward, p_up, &l_right);
			Vector3f_normalize(&l_right);
		}

		Vector3f l_up;
		{
			Vector3f_cross(&l_right, &l_forward, &l_up);
			Vector3f_normalize(&l_up);
		}

		Vector3f l_scale{ 1.0f, 1.0f, 1.0f };
		Matrif4x4_buildTRSV2(p_origin, &l_right, &l_up, &l_forward, &l_scale, p_out);
	};

	void Matrixf4x4_perspective(float p_fov, float p_aspect, float p_near, float p_far, Matrix4x4f* p_out)
	{
		float l_halfTan = tanf(p_fov / 2.0f);

		p_out->_00 = 1.0f / (p_aspect * l_halfTan);
		p_out->_01 = 0.0f;
		p_out->_02 = 0.0f;
		p_out->_03 = 0.0f;

		p_out->_10 = 0.0f;
		p_out->_11 = 1.0f / l_halfTan;
		p_out->_12 = 0.0f;
		p_out->_13 = 0.0f;

		p_out->_20 = 0.0f;
		p_out->_21 = 0.0f;
		p_out->_22 = -(p_far + p_near) / (p_far - p_near);
		p_out->_23 = -1.0f;

		p_out->_30 = 0.0f;
		p_out->_31 = 0.0f;
		p_out->_32 = (-2.0f * p_far * p_near) / (p_far - p_near);
		p_out->_33 = 0.0f;
	}
}