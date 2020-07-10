#include "GenericArray.h"

#include <stdlib.h>
#include <string.h>

#include "ErrorHandling/Errorhandling.h"
#include "Functional/Vector/VectorIterator.h"
#include "Functional/Vector/VectorAccessor.h"

size_t Core_GenericArray_getTotalSize(Core_GenericArray* p_genericArray)
{
    return p_genericArray->ElementSize * p_genericArray->Capacity;
};

size_t Core_GenericArray_getElementOffset(Core_GenericArray* p_genericArray, size_t p_index)
{
    return p_genericArray->ElementSize * p_index;
};

void Core_GenericArray_alloc(Core_GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity)
{
    p_genericArray->ElementSize = p_elementSize;
    p_genericArray->Capacity = p_initialCapacity;
    p_genericArray->Memory = malloc(Core_GenericArray_getTotalSize(p_genericArray));
    p_genericArray->Size = 0;
};

void Core_GenericArray_free(Core_GenericArray* p_genericArray)
{
    free(p_genericArray->Memory);
    p_genericArray->Memory = NULL;
    p_genericArray->Capacity = 0;
    p_genericArray->ElementSize = 0;
    p_genericArray->Size = 0;
};

void* Core_GenericArray_at_unchecked(Core_GenericArray* p_genericArray, size_t p_index)
{
    return (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_index);
};

void* Core_GenericArray_at(Core_GenericArray* p_genericArray, size_t p_index)
{
    if (p_index >= p_genericArray->Size)
    {
        return NULL;
    }
    return Core_GenericArray_at_unchecked(p_genericArray, p_index);
};

bool Core_GenericArray_vectorIterator_moveNext(Core_VectorIterator* p_vectorIterator)
{
    p_vectorIterator->CurrentIndex += 1;
    p_vectorIterator->Current = p_vectorIterator->Array->Functions->Accessor->At(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
    return p_vectorIterator->Current != NULL;
};

static const Core_VectorIterator_function Core_GenericArray_VectorIterator_function = { Core_GenericArray_vectorIterator_moveNext };

Core_VectorIterator Core_GenericArray_buildIterator(Core_GenericArray* p_genericArray)
{
    Core_VectorIterator l_it;
    l_it.Array = p_genericArray;
    l_it.Current = NULL;
    l_it.CurrentIndex = -1;
    l_it.Functions = &Core_GenericArray_VectorIterator_function;
    return l_it;
};

void Core_GenericArray_clear(struct Core_GenericArray* p_genericArray)
{
    p_genericArray->Size = 0;
};

void Core_GenericArray_resize(Core_GenericArray* p_genericArray, size_t p_newCapacity)
{
    if (p_newCapacity > p_genericArray->Capacity)
    {
        void* l_newMemory = realloc(p_genericArray->Memory, p_newCapacity * p_genericArray->ElementSize);
        if (l_newMemory)
        {
            p_genericArray->Memory = l_newMemory;
            p_genericArray->Capacity = p_newCapacity;
        }
    }
}

Core_ReturnCodes Core_GenericArray_pushBack_realloc(Core_GenericArray* p_genericArray, void* p_value)
{
    if (p_genericArray->Size >= p_genericArray->Capacity)
    {
        Core_GenericArray_resize(p_genericArray, p_genericArray->Capacity == 0 ? 1 : (p_genericArray->Capacity * 2));
        ERR_PASS(Core_GenericArray_pushBack_realloc(p_genericArray, p_value), "Core_GenericArray_pushBack_realloc");
    }
    else
    {
        void* p_targetMemory = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_genericArray->Size);
        memcpy(p_targetMemory, p_value, p_genericArray->ElementSize);
        p_genericArray->Size += 1;
    }

    return CR_OK;
}

Core_ReturnCodes Core_GenericArray_pushBack_noRealloc(struct Core_GenericArray* p_genericArray, void* p_value)
{
    if (p_genericArray->Size >= p_genericArray->Capacity)
    {
        ERR_PUSH(CR_OUT_OF_BOUND, "Core_GenericArray_pushBack_noRealloc : impossible to push back. array is already full.");
    }

    Core_GenericArray_pushBack_realloc(p_genericArray, p_value);

    return CR_OK;
};

Core_ReturnCodes Core_GenericArray_swap(struct Core_GenericArray* p_genericArray, size_t p_left, size_t p_right)
{
    if (p_left >= p_genericArray->Size || p_right >= p_genericArray->Size) { ERR_PUSH(CR_OUT_OF_BOUND, "Core_GenericArray_swap : out_of_range"); }
    if (p_left > p_right) { ERR_PUSH(CR_INVALID_INDICES, "Core_GenericArray_swap : invalid indices."); }
    if (p_left == p_right) { return CR_OK; }

    char* l_leftMemoryTarget = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_left);
    char* l_rightMemoryTarget = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_right);

    for (size_t i = 0; i < p_genericArray->ElementSize; i++)
    {
        char l_rightTmp = l_rightMemoryTarget[i];
        l_rightMemoryTarget[i] = l_leftMemoryTarget[i];
        l_leftMemoryTarget[i] = l_rightTmp;
    }

    return CR_OK;
};

Core_ReturnCodes Core_GenericArray_isertAt_realloc(Core_GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index)
{
    if (p_index > p_genericArray->Size)
    {
        ERR_PUSH(CR_OUT_OF_BOUND, "Core_GenericArray_isertAt_realloc : out of range");
    }

    if (p_genericArray->Size + p_elementNb > p_genericArray->Capacity)
    {
        Core_GenericArray_resize(p_genericArray, p_genericArray->Capacity == 0 ? 1 : (p_genericArray->Capacity * 2));
        ERR_PASS(Core_GenericArray_isertAt_realloc(p_genericArray, p_value, p_elementNb, p_index), "Core_GenericArray_isertAt_realloc : insertAt error");
    }
    else
    {
        void* l_initialElement = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_index);
        // If we insert between existing elements, we move down memory to give space for new elements
        if (p_genericArray->Size - p_index > 0)
        {
            void* l_targetElement = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_index + p_elementNb);
            memmove(l_targetElement, l_initialElement, p_genericArray->ElementSize * (p_genericArray->Size - p_index));
        }
        memcpy(l_initialElement, p_value, p_genericArray->ElementSize * p_elementNb);
        p_genericArray->Size += p_elementNb;
    }

    return CR_OK;
};

Core_ReturnCodes Core_GenericArray_isertAt_noRealloc(struct Core_GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index)
{
    if ((p_genericArray->Size + p_elementNb) >= p_genericArray->Capacity)
    {
        ERR_PUSH(CR_OUT_OF_BOUND, "Core_GenericArray_isertAt_neRealloc : inserted array is too large !");
    }

    return Core_GenericArray_isertAt_realloc(p_genericArray, p_value, p_elementNb, p_index);
};

Core_ReturnCodes Core_GenericArray_isertArrayAt_realloc(Core_GenericArray* p_genericArray, Core_GenericArray* p_insertedArray, size_t p_index)
{
    if (p_genericArray->ElementSize != p_insertedArray->ElementSize)
    {
        ERR_PUSH(CR_INVALID_INDICES, "Core_GenericArray_isertAt_realloc : element size doesn't match !");
    }

    ERR_PASS(Core_GenericArray_isertAt_realloc(p_genericArray, p_insertedArray->Memory, p_insertedArray->Size, p_index), "Core_GenericArray_isertAt_realloc : error");

    return CR_OK;
};

Core_ReturnCodes Core_GenericArray_isertArrayAt_noRealloc(struct Core_GenericArray* p_genericArray, struct Core_GenericArray* p_insertedArray, size_t p_index)
{
    if ((p_genericArray->Size + p_insertedArray->Size) >= p_genericArray->Capacity)
    {
        ERR_PUSH(CR_OUT_OF_BOUND, "Core_GenericArray_isertArrayAt_noRealloc : inserted array is too large !");
    }
 
    Core_GenericArray_isertArrayAt_realloc(p_genericArray, p_insertedArray, p_index);

    return CR_OK;
};
