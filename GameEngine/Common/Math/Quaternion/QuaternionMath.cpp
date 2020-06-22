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
		return
			fabsf(p_left->x - p_right->x) < FLOAT_TOLERANCE &&
			fabsf(p_left->y - p_right->y) < FLOAT_TOLERANCE &&
			fabsf(p_left->z - p_right->z) < FLOAT_TOLERANCE &&
			fabsf(p_left->w - p_right->w) < FLOAT_TOLERANCE
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

}