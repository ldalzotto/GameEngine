#pragma once

namespace _GameEngine::_Math
{
	struct Transform;
	struct Box;
}

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		_Math::Transform* Transform;
		_Math::Box* Box;
	};

	bool BoxCollider_equals(BoxCollider** p_left, BoxCollider** p_right, void*);

	BoxCollider* BoxCollider_alloc(BoxCollider* p_template);
	void BoxCollider_free(BoxCollider** p_boxCollider);
}