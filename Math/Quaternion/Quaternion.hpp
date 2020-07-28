#pragma once

#include "Vector/Vector.hpp"
#include "Quaternion.h"

namespace _Math
{
	template <typename T>
	struct Quaternion
	{
		T x;
		T y;
		T z;
		T w = 1;

		inline static Quaternion<T> build(Vector<4, T>& p_quat)
		{
			return *(Quaternion<T>*) & p_vec4;
		}

		inline static Quaternion<T> build(Vector<3, T>& p_vector, T p_scalar)
		{
			return Quaternion<T>
			{
				p_vector.x,
					p_vector.y,
					p_vector.z,
					p_scalar
			};
		};

		inline static Quaternion<T> fromDirection(Vector<3, T>& p_vect)
		{
			Quaternion<T> l_out;
			RQuaternion_fromDirection((T*)(&p_vect), (T*)(&l_out));
			return l_out;
		};

		inline Vector<3, T> vector()
		{
			return *(Vector<3, T>*)this;
		};

		inline Quaternion<T> conjugate()
		{
			Quaternion<T> l_out;
			RQuaternion_conjugate((T*)(this), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> normalize()
		{
			Quaternion<T> l_out;
			RQuaternion_normalize((T*)(this), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> mul(Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_mul((T*)(this), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> cross(Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_cross((T*)(this), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};

		static Quaternion<T> rotateAround(Vector3<T>& p_axis, T p_angle)
		{
			Quaternion<T> l_out;
			RQuaternion_rotateAround((T*)(&p_axis), p_angle, (T*)(&l_out));
			return l_out;
		};
	};

}