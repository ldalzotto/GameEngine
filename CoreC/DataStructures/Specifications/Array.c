#include "Array.h"

#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"

static const Core_VectorAccessor Core_array_accessor = {
        Core_GenericArray_at,
        Core_GenericArray_at_unchecked
};

static const Core_VectorWriter Core_array_writer = {
       Core_GenericArray_clear,
       Core_GenericArray_pushBack_noRealloc,
       Core_GenericArray_swap,
       Core_GenericArray_isertAt_noRealloc,
       Core_GenericArray_isertArrayAt_noRealloc,
       Core_GenericArray_eraseCompare
};

static const Core_GenericArray_functions Core_array_functions =
{
    &Core_array_accessor, &Core_array_writer, Core_GenericArray_buildIterator
};

void Core_Array_alloc(Core_GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity)
{
	Core_GenericArray_alloc(p_genericArray, p_elementSize, p_initialCapacity);
	p_genericArray->Functions = &Core_array_functions;
};

Core_GenericArray Core_array_fromCStyle(void* p_cArray, size_t p_elementSize, size_t p_numberOfElements)
{
    Core_GenericArray l_arr;
    l_arr.Capacity = p_numberOfElements;
    l_arr.Size = p_numberOfElements;
    l_arr.ElementSize = p_elementSize;
    l_arr.Memory = p_cArray;
    l_arr.Functions = &Core_array_functions;

    return l_arr;
};