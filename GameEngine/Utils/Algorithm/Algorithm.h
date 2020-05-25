#pragma once

#include <algorithm>
#include <vector>

namespace _GameEngine::_Utils
{
	template<class T>
	void Vector_eraseElementEquals(std::vector<T>& p_array, T p_element)
	{
		for (size_t i = 0; i < p_array.size(); i++)
		{
			if (p_array.at(i) == p_element)
			{
				p_array.erase(p_array.begin() + i);
				return;
			}
		}
	};

	template<class T>
	size_t Vector_containsElementEquals(std::vector<T>& p_array, T p_element)
	{
		for (size_t i = 0; i < p_array.size(); i++)
		{
			if (p_array.at(i) == p_element)
			{
				return i;
			}
		}
		return -1;
	};
}