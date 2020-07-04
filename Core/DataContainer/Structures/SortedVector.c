#include "SortedVector.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

void Core_SortedVector_pushBack(Core_SortedVector* p_vector, void* p_value)
{
	size_t l_insertIndex = 0;
	for (size_t i = 0; i < p_vector->GenericArray.Size; i++)
	{
		short int l_compareValue = p_vector->SortComparator(p_value, Core_GenericArray_at(&p_vector->GenericArray, i), NULL);
		if (l_compareValue >= 0)
		{
			l_insertIndex = i + 1;
		}
		else
		{
			break;
		}
	}

	Core_GenericArray_isertAt_realloc(&p_vector->GenericArray, p_value, 1, l_insertIndex);
}


void Core_SortedVector_buildIterator(Core_SortedVector* p_vector, struct Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->Core_VectorIterator_DataStructure = p_vector;
	p_vectorIterator->Current = NULL;
	p_vectorIterator->CurrentIndex = -1;
	p_vectorIterator->At = Core_GenericArray_at;
	p_vectorIterator->At_unchecked = Core_GenericArray_at_unchecked;
};

void Core_SortedVector_alloc(Core_SortedVector* p_vector, size_t p_initialCapacity, Core_SortElementComparatorWithUserObject p_sortComparator)
{
	p_vector->Writer.Core_VectorWriter_UserObject = p_vector;
	p_vector->Writer.PushBack = Core_SortedVector_pushBack;
	p_vector->Writer.Swap = Core_GenericArray_swap;

	p_vector->BuildIterator = Core_SortedVector_buildIterator;

	p_vector->SortComparator = p_sortComparator;
	Core_GenericArray_alloc(&p_vector->GenericArray, p_initialCapacity);
};

void Core_SortedVector_free(Core_SortedVector* p_vector)
{
	Core_GenericArray_free(&p_vector->GenericArray);
};
