#pragma once

namespace _GameEngine::_Math
{
	struct Matrix4x4f;
	struct Matrix3x3f;
	struct Quaternionf;
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
	void Matrixf4x4_build(Matrix3x3f* p_matrix, Matrix4x4f* p_out);

	void Matrif4x4_buildTRS(Vector3f* p_position, Quaternionf* p_quaternion, Vector3f* p_scale, Matrix4x4f* out_TRS);
	void Matrif4x4_buildTRS(Vector3f* p_position, Vector3f* p_forward, Vector3f* p_right, Vector3f* p_up, Vector3f* p_scale, Matrix4x4f* out_TRS);

	void Matrixf4x4_buildTranslationMatrix(Vector3f* p_translation, Matrix4x4f* p_out);
	void Matrixf4x4_buildRotationMatrix(Vector3f* p_forward, Vector3f* p_right, Vector3f* p_up, Matrix4x4f* p_out);
	void Matrixf4x4_buildScaleMatrix(Vector3f* p_scale, Matrix4x4f* p_out);

	void Matrixf4x4_mul(Matrix4x4f* p_matrix, float p_value, Matrix4x4f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector4f* p_vector, Vector4f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Matrix4x4f* p_other, Matrix4x4f* out);
	void Matrixf4x4_inv(Matrix4x4f* p_matrix, Matrix4x4f* p_out);

	void Matrixf4x4_lookAtRotation(Vector3f* p_origin, Vector3f* p_target, Vector3f* p_up, Matrix4x4f* p_out);

	void Matrixf4x4_lookAt(Vector3f* p_origin, Vector3f* p_target, Vector3f* p_up, Matrix4x4f* p_out);
	void Matrixf4x4_perspective(float p_fov, float p_aspect, float p_near, float p_far, Matrix4x4f* p_out);

}