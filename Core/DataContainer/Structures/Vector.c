#include "Vector.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

#include <string.h>

size_t Core_Vector_getTotalSize(Core_Vector* p_vector)
{
	return p_vector->GenericArray.ElementSize * p_vector->GenericArray.Capacity;
};

size_t Core_Vector_getElementOffset(Core_Vector* p_vector, size_t p_index)
{
	return p_vector->GenericArray.ElementSize * p_index;
}

void Core_Vector_resize(Core_Vector* p_vector, size_t p_newCapacity)
{
	if (p_newCapacity > p_vector->GenericArray.Capacity)
	{
		void* l_newMemory = realloc(p_vector->GenericArray.Memory, p_newCapacity * p_vector->GenericArray.ElementSize);
		if (l_newMemory)
		{
			p_vector->GenericArray.Memory = l_newMemory;
			p_vector->GenericArray.Capacity = p_newCapacity;
		}
	}
}

void Core_Vector_pushBack(Core_Vector* p_vector, void* p_value)
{
	if (p_vector->GenericArray.Size >= p_vector->GenericArray.Capacity)
	{
		Core_Vector_resize(p_vector, p_vector->GenericArray.Capacity == 0 ? 1 : (p_vector->GenericArray.Capacity * 2));
		Core_Vector_pushBack(p_vector, p_value);
	}
	else
	{
		void* p_targetMemory = (char*)p_vector->GenericArray.Memory + Core_Vector_getElementOffset(p_vector, p_vector->GenericArray.Size);
		memcpy(p_targetMemory, p_value, p_vector->GenericArray.ElementSize);
		p_vector->GenericArray.Size += 1;
	}
}

void* Core_Vector_VectorIterator_at_unchecked(Core_Vector* p_vector, size_t p_index)
{
	return (char*)p_vector->GenericArray.Memory + Core_Vector_getElementOffset(p_vector, p_index);
};

void* Core_Vector_VectorIterator_at(Core_Vector* p_vector, size_t p_index)
{
	if (p_index >= p_vector->GenericArray.Size)
	{
		return NULL;
	}
	return Core_Vector_VectorIterator_at_unchecked(p_vector, p_index);
}

void Core_Vector_buildIterator(Core_Vector* p_vector, Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->Core_VectorIterator_DataStructure = p_vector;
	p_vectorIterator->Current = NULL;
	p_vectorIterator->CurrentIndex = -1;
	p_vectorIterator->At = Core_Vector_VectorIterator_at;
	p_vectorIterator->At_unchecked = Core_Vector_VectorIterator_at_unchecked;
};


void Core_Vector_alloc(Core_Vector* p_vector, size_t p_initialCapacity)
{
	p_vector->Writer.Core_VectorWriter_UserObject = p_vector;
	p_vector->Writer.PushBack = Core_Vector_pushBack;

	p_vector->GenericArray.Capacity = p_initialCapacity;
	p_vector->GenericArray.Memory = malloc(Core_Vector_getTotalSize(p_vector));
	p_vector->GenericArray.Size = 0;
};

void Core_Vector_free(Core_Vector* p_vector)
{
	free(p_vector->GenericArray.Memory);
	p_vector->GenericArray.Memory = NULL;
	p_vector->GenericArray.Capacity = 0;
	p_vector->GenericArray.ElementSize = 0;
	p_vector->GenericArray.Size = 0;
};