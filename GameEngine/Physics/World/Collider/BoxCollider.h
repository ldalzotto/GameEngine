#pragma once

namespace _MathV2
{
	struct Transform;
	struct Box;
}

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		_MathV2::Transform* Transform;
		_MathV2::Box* Box;
	};

	bool BoxCollider_equals(BoxCollider** p_left, BoxCollider** p_right, void*);

	BoxCollider* BoxCollider_alloc(_MathV2::Box* p_box, _MathV2::Transform* ptransform);
	void BoxCollider_free(BoxCollider** p_boxCollider);
}