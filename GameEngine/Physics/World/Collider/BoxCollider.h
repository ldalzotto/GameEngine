#pragma once

#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/TransformC_def.h"

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		Transform_PTR Transform;
		BoxF_PTR Box;
	};

	bool BoxCollider_equals(BoxCollider** p_left, BoxCollider** p_right, void*);

	BoxCollider* BoxCollider_alloc(BoxF_PTR p_box, Transform_PTR ptransform);
	void BoxCollider_free(BoxCollider** p_boxCollider);
}