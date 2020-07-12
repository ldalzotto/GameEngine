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
    Core_GenericArray_pushBack_realloc(p_genericArray, &l_nulChar);
}

void Core_string_alloc(Core_GenericArray* p_genericArray, size_t p_initialCapacity)
{
	Core_GenericArray_alloc(p_genericArray, sizeof(char), p_initialCapacity + 1);
    Core_string_clear(p_genericArray);
};