#include "SortedLinearMap.h"

#include <string.h>
#include "Constants.h"
#include "ErrorHandling/ReturnCodes.h"
#include "ErrorHandling/Errorhandling.h"
#include "Algorithm/Compare/CompareAlgorithm.h"
#include "Algorithm/Sort/SortAlgorithm.h"

void Core_SortedLinearMap_alloc(Core_SortedLinearMap* p_sortedLinearMap, size_t p_elementSize, size_t p_initialCapacity, Core_elementSort_function p_sortFunction, Core_Equals_function p_equalsFunction)
{
	Core_GenericArray_alloc(&p_sortedLinearMap->GenericArray, p_elementSize, p_initialCapacity);
	p_sortedLinearMap->SortFunction = p_sortFunction;
	p_sortedLinearMap->EqualsFunction = p_equalsFunction;
};

void Core_SortedLinearMap_free(Core_SortedLinearMap* p_sortedLinearMap)
{
	Core_GenericArray_free(&p_sortedLinearMap->GenericArray);
};

Core_ReturnCodes Core_SortedLinearMap_pushBack_realloc(Core_SortedLinearMap* p_sortedLinearMap, void* p_value, void** p_out_insertedElement)
{
	*p_out_insertedElement = NULL;

	Core_VectorIterator l_iterator = Core_GenericArray_buildIterator(&p_sortedLinearMap->GenericArray);
	Core_Comparator l_comparator; ZEROING(Core_Comparator, &l_comparator);
	l_comparator.Function = p_sortedLinearMap->EqualsFunction;
	l_comparator.ComparedObject = p_value;

	if (Core_find(&l_iterator, &l_comparator))
	{
		ERR_PUSH(CR_OUT_OF_BOUND, "Core_SortedLinearMap_pushBack_realloc : pushing an already present element.");
	};

	Core_ElementSorter l_sorter; ZEROING(Core_ElementSorter, &l_sorter);
	l_sorter.Function = p_sortedLinearMap->SortFunction;
	size_t l_insertIndex = Core_minIndex(&p_sortedLinearMap->GenericArray, 0, &l_sorter);

	ERR_PASS(Core_GenericArray_isertAt_realloc(&p_sortedLinearMap->GenericArray, p_value, 1, l_insertIndex), "Core_SortedLinearMap_pushBack_realloc : Error while inserting");

	*p_out_insertedElement = Core_GenericArray_at_unchecked(&p_sortedLinearMap->GenericArray, l_insertIndex);
	return CR_OK;
};

bool Core_SortedLinearMap_find(Core_SortedLinearMap* p_sortedLinearMap, void* p_comparedElement, Core_VectorIterator* p_out_vectorIterator)
{
	*p_out_vectorIterator = Core_GenericArray_buildIterator(&p_sortedLinearMap->GenericArray);
	Core_Comparator l_comparator; ZEROING(Core_Comparator, &l_comparator);
	l_comparator.Function = p_sortedLinearMap->EqualsFunction;
	l_comparator.ComparedObject = p_comparedElement;
	return Core_find(p_out_vectorIterator, &l_comparator);
};

Core_SortedLinearMap Core_SortedLinearMap_deepCopy(Core_SortedLinearMap* p_source)
{
	Core_SortedLinearMap l_copiedMap;
	memcpy(&l_copiedMap, p_source, sizeof(Core_SortedLinearMap));
	l_copiedMap.GenericArray = Core_GenericArray_deepCopy(&p_source->GenericArray);
	return l_copiedMap;
};