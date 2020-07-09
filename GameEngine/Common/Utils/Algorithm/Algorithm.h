#pragma once

#include <algorithm>
#include <vector>
#include <limits>

namespace _GameEngine::_Utils
{

	template <class T>
	inline void Hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	};
}