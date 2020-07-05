#include "CompareAlgorithm.h"

#include "DataContainer/FunctionalObjets/VectorIterator.h"
#include "DataContainer/FunctionalObjets/VectorIteratorMacros.h"

void* Core_find(Core_VectorIterator* p_iterator, Core_CompareElementComparatorWithUserObject p_comparator, void* p_other, void* p_userObject)
{
	while (Core_VectorIterator_moveNext(p_iterator))
	{
		if (p_comparator(p_iterator->Current, p_other, p_userObject))
		{
			return p_iterator->Current;
		}
	}
};