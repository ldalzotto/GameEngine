#include "BoxCollider.h"

#include <stdlib.h>

namespace _GameEngine::_Physics
{
	bool BoxCollider_equals(BoxCollider** p_left, BoxCollider** p_right, void*)
	{
		return *p_left == *p_right;
	};

	BoxCollider* BoxCollider_alloc(BoxCollider* p_template)
	{
		BoxCollider* l_boxCollider = (BoxCollider*)malloc(sizeof(BoxCollider));
		l_boxCollider->Box = p_template->Box;
		l_boxCollider->Transform = p_template->Transform;
		return l_boxCollider;
	};
	
	void BoxCollider_free(BoxCollider** p_boxCollider)
	{
		free((*p_boxCollider));
		*p_boxCollider = nullptr;
		p_boxCollider = nullptr;
	};
}