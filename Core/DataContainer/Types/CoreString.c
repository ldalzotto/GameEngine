#include "CoreString.h"

#include "DataContainer/Structures/Vector.h"
#include "DataContainer/Structures/Array.h"
#include "DataContainer/Algorithm/Sort/SortComparators.h"
#include "DataContainer/Algorithm/Sort/SortingAlgorithm.h"

#include "DataContainer/FunctionalObjets/VectorIterator.h"
#include "DataContainer/Algorithm/String/StringAlgorithm.h"

__forceinline void Core_char_Vector_alloc(Core_char_Vector* p_vector, size_t p_initialCapacity)
{
	Core_Vector_alloc((Core_Vector*)p_vector, sizeof(char), p_initialCapacity + 1);
	char l_nul = NULL;
	p_vector->Writer.PushBack(p_vector, &l_nul);
};

__forceinline void Core_char_Vector_free(Core_char_Vector* p_vector)
{
	Core_Vector_free((Core_Vector*)p_vector);
};

// CORE_DEFINE_VECTOR_IMPL(char);
CORE_VECTORWRITER_DEFINE_IMPL(Core_char_Vector, char);
CORE_SORT_ALGORITHM_DEFINE_IMPL(Core_char_Vector, char);
CORE_STRING_ALGORITHM_DEFINE_IMPL(Core_char_Vector);

CORE_DEFINE_ARRAY_IMPL(char);

CORE_VECTORWRITER_DEFINE_IMPL(Core_char_Array, char);
CORE_SORT_ALGORITHM_DEFINE_IMPL(Core_char_Array, char);
CORE_STRING_ALGORITHM_DEFINE_IMPL(Core_char_Array);
