#include "String.h"

#include "DataStructures/GenericArray.h"
#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"

void Core_string_clear(Core_GenericArray* p_genericArray)
{
    if (p_genericArray->Size > 0)
    {
        p_genericArray->Size = 0;
    }
    char l_nulChar = (char)NULL;
    p_genericArray->Functions->Writer->PushBack(p_genericArray, &l_nulChar);
}

static const Core_VectorAccessor Core_string_accessor =
{
	Core_GenericArray_at,
	Core_GenericArray_at_unchecked
};

static const Core_VectorWriter Core_string_writer =
{
    Core_string_clear,
    Core_GenericArray_pushBack_realloc,
    Core_GenericArray_swap,
    Core_GenericArray_isertAt_realloc,
    Core_GenericArray_isertArrayAt_realloc,
    Core_GenericArray_eraseCompare
};

static const Core_GenericArray_functions Core_string_functions =
{
    &Core_string_accessor, &Core_string_writer, Core_GenericArray_buildIterator
};

void Core_string_alloc(Core_GenericArray* p_genericArray, size_t p_initialCapacity)
{
	Core_GenericArray_alloc(p_genericArray, sizeof(char), p_initialCapacity + 1);
    p_genericArray->Functions = &Core_string_functions;
    p_genericArray->Functions->Writer->Clear(p_genericArray);
};