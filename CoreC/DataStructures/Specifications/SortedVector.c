#include "SortedVector.h"

#include <string.h>

#include "Constants.h"
#include "Algorithm/Sort/SortAlgorithm.h"
#include "ErrorHandling/Errorhandling.h"
#include "DataStructures/GenericArray.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"

Core_ReturnCodes Core_SortedVector_pushBack(Core_SortedVector* p_sortedVector, void* p_value)
{
	Core_elementSort_function l_sortFunction = p_sortedVector->SortFunction;

	Core_ElementSorter l_elementSorter; ZEROING(Core_ElementSorter, &l_elementSorter);
	l_elementSorter.Function = p_sortedVector->SortFunction;
	size_t l_insertIndex = Core_minIndex(&p_sortedVector->GenericArray, 0, &l_elementSorter);
	ERR_PASS(Core_GenericArray_isertAt_realloc(&p_sortedVector->GenericArray, p_value, 1, l_insertIndex), "Core_SortedVector_pushBack : Error while inserting");
	return CR_OK;
}

void Core_SortedVector_alloc(Core_SortedVector* p_sortedVector, size_t p_elementSize, size_t p_initialCapacity, Core_elementSort_function p_sortFunction)
{
	Core_GenericArray_alloc(&p_sortedVector->GenericArray, p_elementSize, p_initialCapacity);
	p_sortedVector->SortFunction = p_sortFunction;
};

void Core_SortedVector_free(Core_SortedVector* p_sortedVector)
{
	Core_GenericArray_free(&p_sortedVector->GenericArray);
};

Core_SortedVector Core_SortedVector_deepCopy(Core_SortedVector* p_sortedVector)
{
	Core_SortedVector l_copiedVector;
	memcpy(&l_copiedVector, p_sortedVector, sizeof(Core_SortedVector));
	l_copiedVector.GenericArray = Core_GenericArray_deepCopy(&p_sortedVector->GenericArray);
	return l_copiedVector;
}