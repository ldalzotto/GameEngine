#include "QuaternionMath.h"

#include <string>
#include <math.h>
#include "Quaternion.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"

namespace _GameEngine::_Math
{
	void Quaternionf_copy(Quaternionf* p_source, Quaternionf* p_target)
	{
		memcpy(p_target, p_source, sizeof(Quaternionf));
	};

	bool Quaternionf_equals(Quaternionf* p_left, Quaternionf* p_right)
	{
		return
			p_left->x == p_right->x &&
			p_left->y == p_right->y &&
			p_left->z == p_right->z &&
			p_left->w == p_right->w
			;
	};

	void Quaternion_mul(Quaternionf* p_quaternion, Quaternionf* p_other, Quaternionf* out)
	{
		out->w = (p_quaternion->w * p_other->w) - (p_quaternion->x * p_other->x) - (p_quaternion->y * p_other->y) - (p_quaternion->z * p_other->z);
		out->x = (p_quaternion->w * p_other->x) + (p_quaternion->x * p_other->w) + (p_quaternion->y * p_other->z) - (p_quaternion->z * p_other->y);
		out->y = (p_quaternion->w * p_other->y) + (p_quaternion->y * p_other->w) + (p_quaternion->z * p_other->x) - (p_quaternion->x * p_other->z);
		out->z = (p_quaternion->w * p_other->z) + (p_quaternion->z * p_other->w) + (p_quaternion->x * p_other->y) - (p_quaternion->y * p_other->x);
	};

	void Quaternion_rotateAround(Quaternionf* p_quaternion, Vector3f* p_axis, float p_angle, Quaternionf* out)
	{
		_Math::Quaternionf l_deltaRotation;
		{
			_Math::Vector3f l_deltaEuler;
			{
				_Math::Vector3f_mul(p_axis, p_angle, &l_deltaEuler);
			}
			_Math::Quaternion_fromEulerAngles(&l_deltaEuler, &l_deltaRotation);
		}

		_Math::Quaternion_mul(p_quaternion, &l_deltaRotation, out);
	};

	void Quaternion_fromEulerAngles(Vector3f* p_eulerAngles, Quaternionf* p_out)
	{
		_Math::Vector3f l_cos;
		{
			_Math::Vector3f_mul(p_eulerAngles, 0.5f, &l_cos);
			l_cos.x = cosf(l_cos.x);
			l_cos.y = cosf(l_cos.y);
			l_cos.z = cosf(l_cos.z);
		}

		_Math::Vector3f l_sin;
		{
			_Math::Vector3f_mul(p_eulerAngles, 0.5f, &l_sin);
			l_sin.x = sinf(l_sin.x);
			l_sin.y = sinf(l_sin.y);
			l_sin.z = sinf(l_sin.z);
		}

		p_out->w = l_cos.x * l_cos.y * l_cos.z + l_sin.x * l_sin.y * l_sin.z;
		p_out->x = l_sin.x * l_cos.y * l_cos.z - l_cos.x * l_sin.y * l_sin.z;
		p_out->y = l_cos.x * l_sin.y * l_cos.z + l_sin.x * l_cos.y * l_sin.z;
		p_out->z = l_cos.x * l_cos.y * l_sin.z - l_sin.x * l_sin.y * l_cos.z;
	};

	void Quaternion_fromEulerAngles(Vector3f& p_eulerAngles, Quaternionf* p_out)
	{
		Quaternion_fromEulerAngles(&p_eulerAngles, p_out);
	};

	void Quaterion_toRotationMatrix(Quaternionf* p_quaternion, Matrix3x3f* out_matrix)
	{
		float l_qxx = p_quaternion->x * p_quaternion->x;
		float l_qxy = p_quaternion->x * p_quaternion->y;
		float l_qxz = p_quaternion->x * p_quaternion->z;
		float l_qxw = p_quaternion->x * p_quaternion->w;

		float l_qyy = p_quaternion->y * p_quaternion->y;
		float l_qyz = p_quaternion->y * p_quaternion->z;
		float l_qyw = p_quaternion->y * p_quaternion->w;

		float l_qzz = p_quaternion->z * p_quaternion->z;
		float l_qzw = p_quaternion->z * p_quaternion->w;


		//Right
		out_matrix->_00 = 1 - (2 * l_qyy) - (2 * l_qzz);
		out_matrix->_01 = (2 * l_qxy) + (2 * l_qzw);
		out_matrix->_02 = (2 * l_qxz) - (2 * l_qyw);

		//UP
		out_matrix->_10 = (2 * l_qxy) - (2 * l_qzw);
		out_matrix->_11 = 1 - (2 * l_qxx) - (2 * l_qzz);
		out_matrix->_12 = (2 * l_qyz) + (2 * l_qxw);

		//Forward
		out_matrix->_20 = (2 * l_qxz) + (2 * l_qyw);
		out_matrix->_21 = (2 * l_qyz) - (2 * l_qxw);
		out_matrix->_22 = 1 - (2 * l_qxx) - (2 * l_qyy);
	};

	void Quaterion_toRotationMatrix(Quaternionf* p_quaternion, Matrix4x4f* out_matrix)
	{
		Matrix3x3f l_rotation;
		{
			Quaterion_toRotationMatrix(p_quaternion, &l_rotation);
		}
		Matrixf4x4_build(&l_rotation, out_matrix);
	};
}