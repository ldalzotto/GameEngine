#include "Array.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

void Core_Array_buildIterator(Core_Array* p_array, Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->Core_VectorIterator_DataStructure = p_array;
	p_vectorIterator->Current = NULL;
	p_vectorIterator->CurrentIndex = -1;
};

void Core_Array_alloc(Core_Array* p_array, size_t p_elementSize, size_t p_initialCapacity)
{
	p_array->Writer.Core_VectorWriter_UserObject = p_array;
	p_array->Writer.PushBack = Core_GenericArray_pushBack_noRealloc;
	p_array->Writer.Swap = Core_GenericArray_swap;
	p_array->Writer.InsertArrayAt = Core_GenericArray_isertArrayAt_noRealloc;

	p_array->Accessor.Core_VectorAccessor_DataStructure = p_array;
	p_array->Accessor.At = Core_GenericArray_at;
	p_array->Accessor.At_unchecked = Core_GenericArray_at_unchecked;

	p_array->BuildIterator = Core_Array_buildIterator;

	Core_GenericArray_alloc(&p_array->GenericArray, p_elementSize, p_initialCapacity);
};

void Core_Array_fromRawArray(Core_Array* p_array, void* p_rawArray, size_t p_elementSize, size_t p_elementCount)
{
	p_array->Writer.Core_VectorWriter_UserObject = p_array;
	p_array->Writer.PushBack = Core_GenericArray_pushBack_noRealloc;
	p_array->Writer.Swap = Core_GenericArray_swap;
	p_array->Writer.InsertArrayAt = Core_GenericArray_isertArrayAt_noRealloc;

	p_array->BuildIterator = Core_Array_buildIterator;

	// size_t l_rawArraywElementNb = (sizeof(p_rawArray) / sizeof(*p_rawArray));
	p_array->GenericArray.Capacity = p_elementCount;
	p_array->GenericArray.Size = p_elementCount;
	p_array->GenericArray.Memory = p_rawArray;
	p_array->GenericArray.ElementSize = p_elementSize;
};

void Core_Array_free(Core_Array* p_array)
{
	Core_GenericArray_free(&p_array->GenericArray);
};