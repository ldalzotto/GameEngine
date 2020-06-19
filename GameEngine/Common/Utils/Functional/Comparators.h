#pragma once

namespace _GameEngine::_Utils
{

	short SizeTCompararator(size_t p_left, size_t p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	};

}