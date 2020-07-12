#include "Array.h"

Core_GenericArray Core_array_fromCStyle(void* p_cArray, size_t p_elementSize, size_t p_numberOfElements)
{
    Core_GenericArray l_arr;
    l_arr.Capacity = p_numberOfElements;
    l_arr.Size = p_numberOfElements;
    l_arr.ElementSize = p_elementSize;
    l_arr.Memory = p_cArray;

    return l_arr;
};