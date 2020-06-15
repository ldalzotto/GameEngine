#include "MatrixTransform.h"

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorTransform.h"
#include "Math/Quaternion/Quaternion.h"
#include "Math/Quaternion/QuaternionTransform.h"

namespace _GameEngine::_Math
{

	void Matrif4x4_buildTRS(Vector3f* p_position, Quaternionf* p_quaternion, Vector3f* p_scale, Matrix4x4f* out_TRS)
	{
		Matrix3x3f l_rotation;
		Quaterion_toRotationMatrix(p_quaternion, &l_rotation);
		
		Vector3f l_rotation_c0; Vector3f l_rotation_c1; Vector3f l_rotation_c2;

		l_rotation._c0(&l_rotation_c0);
		l_rotation._c1(&l_rotation_c1);
		l_rotation._c2(&l_rotation_c2);


		Vector3f_mul(&l_rotation_c0, p_scale->x);
		Vector3f_mul(&l_rotation_c1, p_scale->y);
		Vector3f_mul(&l_rotation_c2, p_scale->z);


		Vector4f l_return_col0, l_return_col1, l_return_col2, l_return_col3;
		Vector4f_build(&l_rotation_c0, 0.0f, &l_return_col0);
		Vector4f_build(&l_rotation_c1, 0.0f, &l_return_col1);
		Vector4f_build(&l_rotation_c2, 0.0f, &l_return_col2);
		Vector4f_build(p_position, 1.0f, &l_return_col3);

		Matrix4x4f l_return;
		l_return._set_c0(&l_return_col0);
		l_return._set_c1(&l_return_col1);
		l_return._set_c2(&l_return_col2);
		l_return._set_c3(&l_return_col3);

		/*
		glm::mat3x3 l_rotation = glm::mat3x3(p_quaternion);
		return glm::mat4x4(
			glm::vec4(glm::column(l_rotation, 0) * p_scale.x, 0.0f),
			glm::vec4(glm::column(l_rotation, 1) * p_scale.y, 0.0f),
			glm::vec4(glm::column(l_rotation, 2) * p_scale.z, 0.0f),
			glm::vec4(p_position, 1.0f)
		);
		*/
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

}