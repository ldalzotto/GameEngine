#include "QuaternionMath.h"

#include <string>
#include <math.h>

#include "Quaternion.h"
#include "Math/Math.h"
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
		_Math::Vector3f l_leftEuler, l_rightEuler;
		Quaternion_toEulerAngles(p_left, &l_leftEuler);
		Quaternion_toEulerAngles(p_right, &l_rightEuler);
		return Vector3f_equals(&l_leftEuler, &l_rightEuler);
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

	void Quaternion_toEulerAngles(Quaternionf* p_quaternion, Vector3f* p_eulerAngles)
	{
		float test = p_quaternion->x * p_quaternion->y + p_quaternion->z * p_quaternion->w;
		if (test > 0.4999999999f) { // singularity at north pole
			p_eulerAngles->y = 2.0f * atan2f(p_quaternion->x, p_quaternion->w);
			p_eulerAngles->z = M_PI / 2;
			p_eulerAngles->x = 0;
			return;
		}
		if (test < -0.4999999999f) { // singularity at south pole
			p_eulerAngles->y = -2 * atan2f(p_quaternion->x, p_quaternion->w);
			p_eulerAngles->z = -M_PI / 2;
			p_eulerAngles->x = 0;
			return;
		}

		float sqx = p_quaternion->x * p_quaternion->x;
		float sqy = p_quaternion->y * p_quaternion->y;
		float sqz = p_quaternion->z * p_quaternion->z;
		p_eulerAngles->y = atan2f(2.0f * p_quaternion->y * p_quaternion->w - 2 * p_quaternion->x * p_quaternion->z, 1 - 2 * sqy - 2 * sqz);
		p_eulerAngles->z = asinf(2.0f * test);
		p_eulerAngles->x = atan2f(2.0f * p_quaternion->x * p_quaternion->w - 2 * p_quaternion->y * p_quaternion->z, 1 - 2 * sqx - 2 * sqz);
	};

	void Quaternion_extractAxis(Quaternionf* p_quaternion, Vector3f* out_right, Vector3f* out_up, Vector3f* out_forward)
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

		//RIGHT
		out_right->x = 1 - (2 * l_qyy) - (2 * l_qzz);
		out_right->y = (2 * l_qxy) + (2 * l_qzw);
		out_right->z = (2 * l_qxz) - (2 * l_qyw);
		Vector3f_normalize(out_right);

		//UP
		out_up->x = (2 * l_qxy) - (2 * l_qzw);
		out_up->y = 1 - (2 * l_qxx) - (2 * l_qzz);
		out_up->z = (2 * l_qyz) + (2 * l_qxw);
		Vector3f_normalize(out_up);

		//Forward
		out_forward->x = (2 * l_qxz) + (2 * l_qyw);
		out_forward->y = (2 * l_qyz) - (2 * l_qxw);
		out_forward->z = 1 - (2 * l_qxx) - (2 * l_qyy);
		Vector3f_normalize(out_forward);
	};

	void Quaternion_fromAxis(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Quaternionf* out_quaternion)
	{
		// We calculate the four square roots and get the higher one.
		float qxDiag = fmaxf(1 + p_right->x - p_up->y - p_forward->z, 0.0f);
		float qyDiag = fmaxf(1 + p_up->y - p_right->x - p_forward->z, 0.0f);
		float qzDiag = fmaxf(1 + p_forward->z - p_right->x - p_up->y, 0.0f);
		float qwDiag = fmaxf(1 + p_right->x + p_up->y + p_forward->z, 0.0f);

		int l_diagonalIndex = 0;
		float l_biggestDiagonalValue = qxDiag;
		if (qyDiag > l_biggestDiagonalValue)
		{
			l_biggestDiagonalValue = qyDiag;
			l_diagonalIndex = 1;
		}
		if (qzDiag > l_biggestDiagonalValue)
		{
			l_biggestDiagonalValue = qzDiag;
			l_diagonalIndex = 2;
		}
		if (qwDiag > l_biggestDiagonalValue)
		{
			l_biggestDiagonalValue = qwDiag;
			l_diagonalIndex = 3;
		}

		l_biggestDiagonalValue = 0.5f * sqrtf(l_biggestDiagonalValue);
		float mult = 1 / (4.0f * l_biggestDiagonalValue);

		switch (l_diagonalIndex)
		{
		case 0:
		{
			out_quaternion->x = l_biggestDiagonalValue;
			out_quaternion->y = (p_right->y + p_up->x) * mult;
			out_quaternion->z = (p_forward->x + p_right->z) * mult;
			out_quaternion->w = (p_up->z - p_forward->y) * mult;
		}
		break;
		case 1:
		{
			out_quaternion->x = (p_right->y + p_up->x) * mult;
			out_quaternion->y = l_biggestDiagonalValue;
			out_quaternion->z = (p_up->z + p_forward->y) * mult;
			out_quaternion->w = (p_forward->x - p_right->z) * mult;
		}
		break;
		case 2:
		{
			out_quaternion->x = (p_forward->x + p_right->z) * mult;
			out_quaternion->y = (p_up->z + p_forward->y) * mult;
			out_quaternion->z = l_biggestDiagonalValue;
			out_quaternion->w = (p_right->y - p_up->x) * mult;
		}
		break;
		case 3:
		{
			out_quaternion->x = (p_up->z - p_forward->y) * mult;
			out_quaternion->y = (p_forward->x - p_right->z) * mult;
			out_quaternion->z = (p_right->y - p_up->x) * mult;
			out_quaternion->w = l_biggestDiagonalValue;
		}
		break;
		}

	}
}