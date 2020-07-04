#include "SortingAlgorithm.h"

#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"

void* Core_min(Core_VectorIterator* p_iterator, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject)
{
	void* l_minValue = NULL;
	Core_GenericArray* l_vector = (Core_GenericArray*)p_iterator->Core_VectorIterator_DataStructure;
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

size_t Core_minIndex(Core_VectorIterator* p_iterator, Core_SortElementComparatorWithUserObject p_sortComparator, size_t p_beginIndex, void* p_userObject)
{
	void* l_minValue = NULL;
	Core_GenericArray* l_vector = (Core_GenericArray*)p_iterator->Core_VectorIterator_DataStructure;
	size_t l_minIndex = p_beginIndex;

	if (l_vector->Size > 0)
	{
		l_minValue = p_iterator->At_unchecked(p_iterator->Core_VectorIterator_DataStructure, p_beginIndex);
	}

	for (size_t i = p_beginIndex + 1; i < l_vector->Size; i++)
	{
		void* l_comparedValue = p_iterator->At_unchecked(p_iterator->Core_VectorIterator_DataStructure, i);
		short int l_compareValue = p_sortComparator(l_minValue, l_comparedValue, p_userObject);
		if (l_compareValue >= 0)
		{
			l_minValue = l_comparedValue;
			l_minIndex = i;
		}
	}

	return l_minIndex;
};


void Core_selectionSort(Core_VectorIterator* p_iterator, Core_VectorWriter* p_writer, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject)
{
	Core_GenericArray* l_vector = (Core_GenericArray*)p_iterator->Core_VectorIterator_DataStructure;
	for (size_t i = 0; i < l_vector->Size; i++)
	{
		size_t l_minIndex = Core_minIndex(p_iterator, p_sortComparator, i, p_userObject);
		if (l_minIndex != i)
		{
			p_writer->Swap(p_iterator->Core_VectorIterator_DataStructure, i, l_minIndex);
		}
	}
};