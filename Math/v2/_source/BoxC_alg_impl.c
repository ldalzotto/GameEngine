
#include "v2/_interface/BoxC_alg_impl.h"

#include "v2/_interface/VectorC.h"
#include <math.h>

void Box_Grow_F(BoxF_PTR p_box, const Vector3f_PTR p_growingPoint)
{
	Vector3f l_delta; Vec_Min_3f_3f(p_growingPoint, &p_box->Center, &l_delta);

	if (fabs(l_delta.x) > p_box->Extend.x)
	{
		p_box->Extend.x = fabsf(l_delta.x);
	}
	if (fabs(l_delta.y) > p_box->Extend.y)
	{
		p_box->Extend.y = fabsf(l_delta.y);
	}
	if (fabs(l_delta.z) > p_box->Extend.z)
	{
		p_box->Extend.z = fabsf(l_delta.z);
	}
};
