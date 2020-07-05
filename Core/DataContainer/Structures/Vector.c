#include "Vector.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

void Core_Vector_buildIterator(Core_Vector* p_vector, Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->Core_VectorIterator_DataStructure = p_vector;
	p_vectorIterator->Current = NULL;
	p_vectorIterator->CurrentIndex = -1;
	p_vectorIterator->At = Core_GenericArray_at;
	p_vectorIterator->At_unchecked = Core_GenericArray_at_unchecked;
};

void Core_Vector_alloc(Core_Vector* p_vector, size_t p_elementSize, size_t p_initialCapacity)
{
	p_vector->Writer.Core_VectorWriter_UserObject = p_vector;
	p_vector->Writer.PushBack = Core_GenericArray_pushBack_realloc;
	p_vector->Writer.Swap = Core_GenericArray_swap;
	p_vector->Writer.InsertArrayAt = Core_GenericArray_isertArrayAt_realloc;

	p_vector->BuildIterator = Core_Vector_buildIterator;

	Core_GenericArray_alloc(&p_vector->GenericArray, p_elementSize, p_initialCapacity);
};

void Core_Vector_free(Core_Vector* p_vector)
{
	Core_GenericArray_free(&p_vector->GenericArray);
};