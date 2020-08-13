#include "SortAlgorithm.hpp"

#include "Functional/Sort/ElementSorter.hpp"
#include "Functional/Iterator/Iterator.hpp"
#include "DataStructures/GenericArray.hpp"
#include <string.h>
namespace _Core
{
	void Sort_min_findElementIndex(VectorIterator* p_vectorIterator, void* p_comparedElement, size_t p_startIndex, ElementSorter* p_sortComparator)
	{
		size_t l_minIndex = p_startIndex;

		if (p_startIndex != 0)
		{
			VectorIterator_moveToIndex(p_vectorIterator, p_startIndex - 1);
		}

		while (VectorIterator_moveNext(p_vectorIterator))
		{
			void* l_comparedValue = p_vectorIterator->Current;
			short int l_compareValue = p_sortComparator->Function(p_comparedElement, l_comparedValue, p_sortComparator->UserObject);
			if (l_compareValue >= 0)
			{
				l_minIndex += 1;
			}
			else
			{
				break;
			}
		}

		p_vectorIterator->Current = nullptr;
		p_vectorIterator->CurrentIndex = l_minIndex;
	};

	void Sort_min(VectorIterator* p_vectorIterator, size_t p_startIndex, ElementSorter* p_sortComparator)
	{
		size_t l_minIndex = p_startIndex;
		void* l_minValue = nullptr;

		if (p_vectorIterator->Array->Size > 0)
		{
			l_minValue = GenericArray_at_unchecked(p_vectorIterator->Array, l_minIndex);
		}

		if (p_startIndex != 0)
		{
			VectorIterator_moveToIndex(p_vectorIterator, p_startIndex - 1);
		}

		while (VectorIterator_moveNext(p_vectorIterator))
		{
			void* l_comparedValue = p_vectorIterator->Current;
			short int l_compareValue = p_sortComparator->Function(l_minValue, l_comparedValue, p_sortComparator->UserObject);
			if (l_compareValue >= 0)
			{
				l_minIndex = p_vectorIterator->CurrentIndex;
				l_minValue = l_comparedValue;
			}
		}

		p_vectorIterator->Current = (char*)l_minValue;
		p_vectorIterator->CurrentIndex = l_minIndex;
	};

	void Sort_selectionSort(VectorIterator* p_vectorIterator, ElementSorter* p_sortComparator)
	{
		VectorIterator l_initialVectorIterator = *p_vectorIterator;

		while (VectorIterator_moveNext(p_vectorIterator))
		{
			VectorIterator l_minIteratro;
			memcpy(&l_minIteratro, &l_initialVectorIterator, sizeof(VectorIterator));

			Sort_min(&l_minIteratro, p_vectorIterator->CurrentIndex, p_sortComparator);
			if (l_minIteratro.CurrentIndex != p_vectorIterator->CurrentIndex)
			{
				GenericArray_swap(p_vectorIterator->Array, p_vectorIterator->CurrentIndex, l_minIteratro.CurrentIndex);
			}
		}
	};
}
