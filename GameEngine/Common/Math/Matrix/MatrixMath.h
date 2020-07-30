#pragma once

namespace _GameEngine::_Math
{
	struct Matrix4x4f;
	struct Matrix3x3f;
	struct Vector2f;
	struct Vector3f;
	struct Vector4f;
}

namespace _MathV2
{
	template <typename T>
	struct Quaternion;
}

namespace _GameEngine::_Math
{
	void Matrix4x4f_right(Matrix4x4f* p_matrix, Vector4f* p_out);
	void Matrix4x4f_up(Matrix4x4f* p_matrix, Vector4f* p_out);
	void Matrix4x4f_forward(Matrix4x4f* p_matrix, Vector4f* p_out);

	void Matrix4x4f_right(Matrix4x4f* p_matrix, Vector3f* p_out);
	void Matrix4x4f_up(Matrix4x4f* p_matrix, Vector3f* p_out);
	void Matrix4x4f_forward(Matrix4x4f* p_matrix, Vector3f* p_out);

	void Matrixf4x4_build(Matrix3x3f* p_matrix, Matrix4x4f* p_out);

	void Matrif4x4_buildTRS(Vector3f* p_position, _MathV2::Quaternion<float>& p_quaternion, Vector3f* p_scale, Matrix4x4f* out_TRS);
	void Matrif4x4_buildTRSV2(Vector3f* p_position, Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Vector3f* p_scale, Matrix4x4f* out_TRS);

	void Matrixf4x4_buildTranslationMatrix(Vector3f* p_translation, Matrix4x4f* p_out);
	void Matrixf4x4_buildRotationMatrixV2(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Matrix4x4f* p_out);
	void Matrixf4x4_buildScaleMatrix(Vector3f* p_scale, Matrix4x4f* p_out);

	void Matrixf4x4_extractTranslation(Matrix4x4f* p_mat, Vector4f* out_translation);
	void Matrixf4x4_extractScale(Matrix4x4f* p_mat, Vector4f* out_scale);

	void Matrix4x4f_clipToWorld(Matrix4x4f* p_clipToWorldMatrix, Vector3f* p_clipPosition, Vector3f* out_worldPosition);
	void Matrix4x4f_worldToClip(Matrix4x4f* p_worldToClipmatrix, Vector3f* p_worldPosition, Vector3f* out_clipPosition);

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, float p_value, Matrix4x4f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector4f* p_vector, Vector4f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector3f* p_vector, Vector3f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector2f* p_vector, Vector3f* out);
	void Matrixf4x4_inv(Matrix4x4f* p_matrix, Matrix4x4f* p_out);

	void Matrixf4x4_lookAt(Vector3f* p_origin, Vector3f* p_target, Vector3f* p_up, Matrix4x4f* p_out);
	void Matrixf4x4_perspective(float p_fov, float p_aspect, float p_near, float p_far, Matrix4x4f* p_out);
	float Matrixf4x4_far(Matrix4x4f* p_matrix);
	float Matrixf4x4_near(Matrix4x4f* p_matrix);


	void Matrix3x3f_set_c0(Matrix3x3f* p_matrix, Vector3f* p_col);
	void Matrix3x3f_set_c1(Matrix3x3f* p_matrix, Vector3f* p_col);
	void Matrix3x3f_set_c2(Matrix3x3f* p_matrix, Vector3f* p_col);

	void Matrix3x3f_mul(Matrix3x3f* p_matrix, Vector3f* p_vector, Vector3f* p_out);
	void Matrixf3x3_buildRotationMatrixV2(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Matrix3x3f* p_out);
}