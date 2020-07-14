#pragma once

#include "Functional/Iterator/Iterator.hpp"
#include "Functional/Comparator/Comaprator.hpp"

namespace _Core
{
	bool Compare_find(VectorIterator* p_iterator, const Comparator* p_comparator)
	{
		while (VectorIterator_moveNext(p_iterator))
		{
			if (p_comparator->Function(p_iterator->Current, p_comparator->ComparedObject, p_comparator->UserObject))
			{
				return true;
			}
		}

		p_iterator->Current = nullptr;
		p_iterator->CurrentIndex = -1;
		return false;
	};
	
	bool Compare_contains(VectorIterator* p_iterator, const Comparator* p_comparator)
	{
		return Compare_find(p_iterator, p_comparator);
	};
}