#include "String.h"

#include "DataContainer/Structures/Vector.h"
#include "DataContainer/Structures/Array.h"
#include "DataContainer/Algorithm/SortComparators.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

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
CORE_ALGORITHM_DEFINE_IMPL(Core_char_Vector, char);


CORE_DEFINE_ARRAY_IMPL(char);

CORE_VECTORWRITER_DEFINE_IMPL(Core_char_Array, char);
CORE_ALGORITHM_DEFINE_IMPL(Core_char_Array, char);

