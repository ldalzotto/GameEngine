#pragma once

#include <algorithm>
#include <vector>

namespace _GameEngine::_Utils
{
	template<class T>
	bool Vector_eraseElementEquals(std::vector<T>& p_array, T p_element)
	{
		for (size_t i = 0; i < p_array.size(); i++)
		{
			if (p_array.at(i) == p_element)
			{
				p_array.erase(p_array.begin() + i);
				return true;
			}
		}
		return false;
	};

	template<class T>
	size_t Vector_containsElementEquals(std::vector<T>* p_array, T p_element)
	{
		for (size_t i = 0; i < p_array->size(); i++)
		{
			if (p_array->at(i) == p_element)
			{
				return i;
			}
		}
		return -1;
	};


	template<class T>
	size_t Vector_containsElementEquals(std::vector<T>& p_array, T p_element)
	{
		return Vector_containsElementEquals(&p_array, p_element);
	};

	template <class T>
	inline void Hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	};
}