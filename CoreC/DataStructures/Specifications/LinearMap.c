#include "LinearMap.h"

#include "Constants.h"
#include "ErrorHandling/Errorhandling.h"
#include "Algorithm/Compare/CompareAlgorithm.h"

void Core_LinearMap_alloc(Core_LinearMap* p_linearMap, size_t p_elementSize, size_t p_initialCapacity, Core_Equals_function p_equalsFunction)
{
	Core_GenericArray_alloc(&p_linearMap->GenericArray, p_elementSize, p_initialCapacity);
	p_linearMap->EqualsFunction = p_equalsFunction;
};

void Core_LinearMap_free(Core_LinearMap* p_linearMap)
{
	Core_GenericArray_free(&p_linearMap->GenericArray);
};

Core_ReturnCodes Core_LinearMap_pushBack_realloc(Core_LinearMap* p_linearMap, void* p_element)
{
	Core_VectorIterator l_iterator = Core_GenericArray_buildIterator(&p_linearMap->GenericArray);
	Core_Comparator l_comparator; ZEROING(Core_Comparator, &l_comparator);
	l_comparator.Function = p_linearMap->EqualsFunction;
	if (Core_find(&l_iterator, &l_comparator))
	{
		ERR_PUSH(CR_OUT_OF_BOUND, "Core_LinearMap_pushBack_realloc : pushing an already present element.");
	};

	ERR_PASS(Core_GenericArray_pushBack_realloc(&p_linearMap->GenericArray, p_element), "Core_LinearMap_pushBack_realloc : error while inserting element.");

	return CR_OK;
};