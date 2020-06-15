#pragma once

namespace _GameEngine::_Math
{
	struct Matrix4x4f;
	struct Quaternionf;
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
	void Matrif4x4_buildTRS(Vector3f* p_position, Quaternionf* p_quaternion, Vector3f* p_scale, Matrix4x4f* out_TRS);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Vector4f* p_vector, Vector4f* out);
	void Matrixf4x4_mul(Matrix4x4f* p_matrix, Matrix4x4f* p_other, Matrix4x4f* out);
}