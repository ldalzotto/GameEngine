#include "MatrixMath.h"

#include <math.h>

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Quaternion/QuaternionMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

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
		_Math::Vector4f l_vector4f;
		_Math::Vector4f_build(p_vector, 1.0f, &l_vector4f);

		_Math::Vector4f l_out4f;
		Matrixf4x4_mul(p_matrix, &l_vector4f, &l_out4f);

		*out = { l_out4f.x, l_out4f.y, l_out4f.z };
	};

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector2f* p_vector, Vector3f* out)
	{
		_Math::Vector4f l_vector4f;
		_Math::Vector4f_build(p_vector, 1.0f, 1.0f, &l_vector4f);

		_Math::Vector4f l_out4f;
		Matrixf4x4_mul(p_matrix, &l_vector4f, &l_out4f);

		*out = { l_out4f.x, l_out4f.y, l_out4f.z };
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

	void Matrix4x4f_clipToWorld(Matrix4x4f* p_clipToWorldMatrix, Vector3f* p_clipPosition, Vector3f* out_worldPosition)
	{
		Vector4f l_clipPosition4f, l_worldPosition4f;
		{
			Vector4f_build(p_clipPosition, 1.0f, &l_clipPosition4f);
			Matrixf4x4_mul(p_clipToWorldMatrix, &l_clipPosition4f, &l_worldPosition4f);
			Vector4f_homogenize_w(&l_worldPosition4f, &l_worldPosition4f);
		}
		Vector3f_build(&l_worldPosition4f, out_worldPosition);
	};

	void Matrix4x4f_worldToClip(Matrix4x4f* p_worldToClipmatrix, Vector3f* p_worldPosition, Vector3f* out_clipPosition)
	{
		Vector4f l_clipPosition4f, l_worldPosition4f;
		{
			Vector4f_build(p_worldPosition, 1.0f, &l_worldPosition4f);
			Matrixf4x4_mul(p_worldToClipmatrix, &l_worldPosition4f, &l_clipPosition4f);
			Vector4f_mul(&l_clipPosition4f, 1.0f / l_clipPosition4f.w, &l_clipPosition4f);
		}
		Vector3f_build(&l_clipPosition4f, out_clipPosition);
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

		*p_out = *(_Math::Matrix4x4f*) & _MathV2::MatrixM::buildTRS(
			*(_MathV2::Vector3<float>*)(p_origin),
			*(_MathV2::Vector3<float>*) & l_right, *(_MathV2::Vector3<float>*) & l_up, *(_MathV2::Vector3<float>*) & l_forward,
			_MathV2::Vector3<float>{1.0f, 1.0f, 1.0f}
		);
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

	};

	float Matrixf4x4_far(Matrix4x4f* p_matrix)
	{
		float l_alpha = (-1 + p_matrix->_22) / (p_matrix->_22 + 1);
		return l_alpha * ((p_matrix->_32 * (l_alpha - 1.0f) / (-2.0f * l_alpha)));
	};

	float Matrixf4x4_near(Matrix4x4f* p_matrix)
	{
		float l_alpha = (-1 + p_matrix->_22) / (p_matrix->_22 + 1);
		return ((p_matrix->_32 * (l_alpha - 1.0f) / (-2.0f * l_alpha)));
	};


	void Matrix3x3f_set_c0(Matrix3x3f* p_matrix, Vector3f* p_col) { p_matrix->_00 = p_col->x; p_matrix->_01 = p_col->y; p_matrix->_02 = p_col->z; };
	void Matrix3x3f_set_c1(Matrix3x3f* p_matrix, Vector3f* p_col) { p_matrix->_10 = p_col->x; p_matrix->_11 = p_col->y; p_matrix->_12 = p_col->z; };
	void Matrix3x3f_set_c2(Matrix3x3f* p_matrix, Vector3f* p_col) { p_matrix->_20 = p_col->x; p_matrix->_21 = p_col->y; p_matrix->_22 = p_col->z; };

	void Matrix3x3f_mul(Matrix3x3f* p_matrix, Vector3f* p_vector, Vector3f* p_out)
	{
		p_out->x = (p_matrix->_00 * p_vector->x) + (p_matrix->_10 * p_vector->y) + (p_matrix->_20 * p_vector->z);
		p_out->y = (p_matrix->_01 * p_vector->x) + (p_matrix->_11 * p_vector->y) + (p_matrix->_21 * p_vector->z);
		p_out->z = (p_matrix->_02 * p_vector->x) + (p_matrix->_12 * p_vector->y) + (p_matrix->_22 * p_vector->z);
	};

	void Matrixf3x3_buildRotationMatrixV2(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Matrix3x3f* p_out)
	{
		Matrix3x3f_set_c0(p_out, p_right);
		Matrix3x3f_set_c1(p_out, p_up);
		Matrix3x3f_set_c2(p_out, p_forward);
	};
}