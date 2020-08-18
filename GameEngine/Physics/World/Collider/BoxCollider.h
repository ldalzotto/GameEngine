#pragma once

#include "v2/_interface/BoxC_def.h"

namespace _MathV2
{
	struct Transform;
}

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		_MathV2::Transform* Transform;
		BOXF_PTR Box;
	};

	bool BoxCollider_equals(BoxCollider** p_left, BoxCollider** p_right, void*);

	BoxCollider* BoxCollider_alloc(BOXF_PTR p_box, _MathV2::Transform* ptransform);
	void BoxCollider_free(BoxCollider** p_boxCollider);
}