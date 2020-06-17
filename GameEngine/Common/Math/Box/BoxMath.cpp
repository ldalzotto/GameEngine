#include "BoxMath.h"

#include <math.h>
#include "Math/Box/Box.h"
#include "Math/Vector/VectorMath.h"

namespace _GameEngine::_Math
{
	void box_grow(Box* p_box, Vector3f* p_growingPoint)
	{
		Vector3f l_delta;
		{
			Vector3f_min(p_growingPoint, &p_box->Center, &l_delta);
		}

		if (fabs(l_delta.x) > p_box->Extend.x)
		{
			p_box->Extend.x = fabs(l_delta.x);
		}
		if (fabs(l_delta.y) > p_box->Extend.y)
		{
			p_box->Extend.y = fabs(l_delta.y);
		}
		if (fabs(l_delta.z) > p_box->Extend.z)
		{
			p_box->Extend.z = fabs(l_delta.z);
		}
	};

	void Box_build(Box* p_box, _Core::VectorT<_Math::Vector3f>* p_points)
	{
		//calculate center
		{
			p_box->Center = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < p_points->size(); i++)
			{
				_Math::Vector3f_add(&p_box->Center, p_points->at(i), &p_box->Center);
			}
			_Math::Vector3f_mul(&p_box->Center, 1.0f / p_points->size(), &p_box->Center);
		}

		{
			for (size_t i = 0; i < p_points->size(); i++)
			{
				box_grow(p_box, p_points->at(i));
			}
		}
	};

}