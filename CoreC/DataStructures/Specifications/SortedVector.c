#include "SortedVector.h"

#include "ErrorHandling/Errorhandling.h"
#include "DataStructures/GenericArray.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"

Core_ReturnCodes Core_SortedVector_pushBack(Core_GenericArray* p_genericArray, void* p_value)
{
	Core_elementSort_function l_sortFunction = ((Core_SortedVector*)p_genericArray)->SortFunction;

	size_t l_insertIndex = 0;
	for (size_t i = 0; i < p_genericArray->Size; i++)
	{
		short int l_compareValue = l_sortFunction(p_value, Core_GenericArray_at(p_genericArray, i), NULL);
		if (l_compareValue >= 0)
		{
			l_insertIndex = i + 1;
		}
		else
		{
			break;
		}
	}

	ERR_PASS(Core_GenericArray_isertAt_realloc(p_genericArray, p_value, 1, l_insertIndex), "Core_SortedVector_pushBack : Error while inserting");
	return CR_OK;
}

static const Core_VectorAccessor Core_sortedVector_accessor = {
		Core_GenericArray_at,
		Core_GenericArray_at_unchecked
};

static const Core_VectorWriter Core_sortedVector_writer = {
	   Core_GenericArray_clear,
	   Core_SortedVector_pushBack,
	   Core_GenericArray_swap,
	   NULL, //TODO
	   NULL //TODO
};

static const Core_GenericArray_functions Core_sortedVector_functions =
{
	&Core_sortedVector_accessor, &Core_sortedVector_writer, Core_GenericArray_buildIterator
};

void Core_SortedVector_alloc(Core_SortedVector* p_sortedVector, size_t p_elementSize, size_t p_initialCapacity, Core_elementSort_function p_sortFunction)
{
	Core_GenericArray_alloc(&p_sortedVector->GenericArray, p_elementSize, p_initialCapacity);
	p_sortedVector->GenericArray.Functions = &Core_sortedVector_functions;
	p_sortedVector->SortFunction = p_sortFunction;
};

void Core_SortedVector_free(Core_SortedVector* p_sortedVector)
{
	Core_GenericArray_free(&p_sortedVector->GenericArray);
};