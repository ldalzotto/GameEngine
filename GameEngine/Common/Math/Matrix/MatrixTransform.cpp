#include "MatrixTransform.h"

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorTransform.h"
#include "Math/Quaternion/Quaternion.h"
#include "Math/Quaternion/QuaternionTransform.h"

namespace _GameEngine::_Math
{
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


		Matrix3x3f l_rotation;
		Quaterion_toRotationMatrix(p_quaternion, &l_rotation);

		Vector3f l_rotation_c0, l_rotation_c1, l_rotation_c2;

		l_rotation._c0(&l_rotation_c0);
		l_rotation._c1(&l_rotation_c1);
		l_rotation._c2(&l_rotation_c2);

		Vector4f l_return_col0, l_return_col1, l_return_col2, l_return_col3;
		Vector4f_build(&l_rotation_c0, 0.0f, &l_return_col0);
		Vector4f_build(&l_rotation_c1, 0.0f, &l_return_col1);
		Vector4f_build(&l_rotation_c2, 0.0f, &l_return_col2);

		out_TRS->_set_c0(&l_return_col0);
		out_TRS->_set_c1(&l_return_col1);
		out_TRS->_set_c2(&l_return_col2);

		Matrixf4x4_buildScaleMatrix(p_scale, out_TRS);
	};

	void Matrif4x4_buildTRS(Vector3f* p_position, Vector3f* p_forward, Vector3f* p_right, Vector3f* p_up, Vector3f* p_scale, Matrix4x4f* out_TRS)
	{
		out_TRS->_03 = 0.0f;
		out_TRS->_13 = 0.0f;
		out_TRS->_23 = 0.0f;
		out_TRS->_33 = 1.0f;

		Matrixf4x4_buildTranslationMatrix(p_position, out_TRS);
		Matrixf4x4_buildRotationMatrix(p_forward, p_right, p_up, out_TRS);
		Matrixf4x4_buildScaleMatrix(p_scale, out_TRS);
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector4f* p_vector, Vector4f* out)
	{
		out->x = (p_matrix->_00 * p_vector->x) + (p_matrix->_10 * p_vector->y) + (p_matrix->_20 * p_vector->z) + (p_matrix->_30 * p_vector->w);
		out->y = (p_matrix->_01 * p_vector->x) + (p_matrix->_11 * p_vector->y) + (p_matrix->_21 * p_vector->z) + (p_matrix->_31 * p_vector->w);
		out->z = (p_matrix->_02 * p_vector->x) + (p_matrix->_12 * p_vector->y) + (p_matrix->_22 * p_vector->z) + (p_matrix->_32 * p_vector->w);
		out->w = (p_matrix->_03 * p_vector->x) + (p_matrix->_13 * p_vector->y) + (p_matrix->_23 * p_vector->z) + (p_matrix->_33 * p_vector->w);
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

	void Matrixf4x4_inv(Matrix4x4f* p_matrix, Matrix4x4f* p_out)
	{
		
	};

	void Matrixf4x4_buildTranslationMatrix(Vector3f* p_translation, Matrix4x4f* p_out)
	{
		p_out->_set_c3(p_translation);
	}

	void Matrixf4x4_buildRotationMatrix(Vector3f* p_forward, Vector3f* p_right, Vector3f* p_up, Matrix4x4f* p_out)
	{
		p_out->_set_c0(p_forward);
		p_out->_set_c1(p_right);
		p_out->_set_c2(p_up);
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

		Vector3f l_scale{1.0f, 1.0f, 1.0f};
		Matrif4x4_buildTRS(p_origin, &l_forward, &l_right, &l_up, &l_scale, p_out);
	};

}