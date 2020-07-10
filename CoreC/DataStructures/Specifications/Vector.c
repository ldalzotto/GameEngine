#include "Vector.h"

#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"

static const Core_VectorAccessor Core_vector_accessor = {
    Core_GenericArray_at,
    Core_GenericArray_at_unchecked
};

static const Core_VectorWriter Core_vector_writer = {
       Core_GenericArray_clear,
       Core_GenericArray_pushBack_realloc,
       Core_GenericArray_swap,
       Core_GenericArray_isertAt_realloc,
       Core_GenericArray_isertArrayAt_realloc
};

static const Core_GenericArray_functions Core_vector_functions =
{
    &Core_vector_accessor, & Core_vector_writer, Core_GenericArray_buildIterator
};

void Core_Vector_alloc(struct Core_GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity)
{
    Core_GenericArray_alloc(p_genericArray, p_elementSize, p_initialCapacity);
    p_genericArray->Functions = &Core_vector_functions;
};