#include "SortAlgorithm.h"

#include "ErrorHandling/Errorhandling.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"
#include "DataStructures/GenericArray.h"
#include "Functional/Sort/ElementSorter.h"

void* Core_min(struct Core_GenericArray* p_genericArray, struct Core_ElementSorter* p_sortComparator)
{
	void* l_minValue = NULL;
	if (p_genericArray->Size > 0)
	{
		l_minValue = Core_GenericArray_at_unchecked(p_genericArray, 0);
	}

	for (size_t i = 1; i < p_genericArray->Size; i++)
	{
		void* l_comparedValue = Core_GenericArray_at(p_genericArray, i);
		short int l_compareValue = p_sortComparator->Function(l_minValue, l_comparedValue, p_sortComparator->UserObject);
		if (l_compareValue >= 0)
		{
			l_minValue = l_comparedValue;
		}
	}

	return l_minValue;
};

size_t Core_minIndex(struct Core_GenericArray* p_genericArray, size_t p_beginIndex, struct Core_ElementSorter* p_sortComparator)
{
	void* l_minValue = NULL;
	size_t l_minIndex = p_beginIndex;

	if (p_genericArray->Size > 0)
	{
		l_minValue = Core_GenericArray_at_unchecked(p_genericArray, p_beginIndex);
	}

	for (size_t i = p_beginIndex + 1; i < p_genericArray->Size; i++)
	{
		void* l_comparedValue = Core_GenericArray_at_unchecked(p_genericArray, i);
		short int l_compareValue = p_sortComparator->Function(l_minValue, l_comparedValue, p_sortComparator->UserObject);
		if (l_compareValue >= 0)
		{
			l_minValue = l_comparedValue;
			l_minIndex = i;
		}
	}

	return l_minIndex;
};

void Core_selectionSort(struct Core_GenericArray* p_genericArray, struct Core_ElementSorter* p_sortComparator)
{
	for (size_t i = 0; i < p_genericArray->Size; i++)
	{
		size_t l_minIndex = Core_minIndex(p_genericArray, i, p_sortComparator);
		if (l_minIndex != i)
		{
			ERR_THROW(Core_GenericArray_swap(p_genericArray, i, l_minIndex));
		}
	}
};
