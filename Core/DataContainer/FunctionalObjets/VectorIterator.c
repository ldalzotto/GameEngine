#include "VectorIterator.h"
#include "DataContainer/Structures/Vector.h"

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator)
{
	p_iterator->CurrentIndex += 1;
	p_iterator->Current = p_iterator->At(p_iterator->Core_VectorIterator_DataStructure, p_iterator->CurrentIndex);
	return p_iterator->Current != NULL;
};

void* Core_VectorIterator_min(Core_VectorIterator* p_iterator, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject)
{
	void* l_minValue = NULL;
	Core_Vector* l_vector = (Core_Vector*)p_iterator->Core_VectorIterator_DataStructure;
	if (l_vector->Size > 0)
	{
		l_minValue = p_iterator->At_unchecked(p_iterator->Core_VectorIterator_DataStructure, 0);
	}

	for (size_t i = 1; i < l_vector->Size; i++)
	{
		void* l_comparedValue = p_iterator->At_unchecked(p_iterator->Core_VectorIterator_DataStructure, i);
		short int l_compareValue = p_sortComparator(l_minValue, l_comparedValue, p_userObject);
		if (l_compareValue >= 0)
		{
			l_minValue = l_comparedValue;
		}
	}

	return l_minValue;
};