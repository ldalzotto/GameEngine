#include "Quaternion.h"
#include <string>

namespace _GameEngine::_Math
{
	void Quaternionf_copy(Quaternionf* p_source, Quaternionf* p_target)
	{
		memcpy(p_target, p_source, sizeof(Quaternionf));
	};

	bool Quaternionf_equals(Quaternionf* p_left, Quaternionf* p_right)
	{
		return
			p_left->x == p_right->x &&
			p_left->y == p_right->y &&
			p_left->z == p_right->z &&
			p_left->w == p_right->w
			;
	};
}