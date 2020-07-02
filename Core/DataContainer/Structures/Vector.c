#include "Vector.h"
#include "DataContainer/FunctionalObjets/VectorIterator.h"

#include <string.h>

size_t Core_Vector_getTotalSize(Core_Vector* p_vector)
{
	return p_vector->ElementSize * p_vector->Capacity;
};

size_t Core_Vector_getElementOffset(Core_Vector* p_vector, size_t p_index)
{
	return p_vector->ElementSize * p_index;
}

void Core_Vector_resize(Core_Vector* p_vector, size_t p_newCapacity)
{
	if (p_newCapacity > p_vector->Capacity)
	{
		void* l_newMemory = realloc(p_vector->Memory, p_newCapacity * p_vector->ElementSize);
		if (l_newMemory)
		{
			p_vector->Memory = l_newMemory;
			p_vector->Capacity = p_newCapacity;
		}
	}
}

void Core_Vector_pushBack(void* p_vector, void* p_value)
{
	Core_Vector* l_vector = (Core_Vector*)p_vector;
	if (l_vector->Size >= l_vector->Capacity)
	{
		Core_Vector_resize(p_vector, l_vector->Capacity == 0 ? 1 : (l_vector->Capacity * 2));
		Core_Vector_pushBack(p_vector, p_value);
	}
	else
	{
		void* p_targetMemory = (char*)l_vector->Memory + Core_Vector_getElementOffset(l_vector, l_vector->Size);
		memcpy(p_targetMemory, p_value, l_vector->ElementSize);
		l_vector->Size += 1;
	}
}

void* Core_Vector_at_unchecked(Core_Vector* p_vector, size_t p_index)
{
	return (char*)p_vector->Memory + Core_Vector_getElementOffset(p_vector, p_index);
};

void* Core_Vector_VectorIterator_getNextElement(void* p_vector, size_t p_index)
{
	Core_Vector* l_vector = (Core_Vector*)p_vector;
	if (p_index >= l_vector->Size)
	{
		return NULL;
	}
	return Core_Vector_at_unchecked(l_vector, p_index);
}

void Core_Vector_buildIterator(Core_Vector* p_vector, Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->Core_VectorIterator_UserObject = p_vector;
	p_vectorIterator->Current = NULL;
	p_vectorIterator->CurrentIndex = -1;
	p_vectorIterator->GetElement = Core_Vector_VectorIterator_getNextElement;
};


void Core_Vector_alloc(Core_Vector* p_vector, size_t p_initialCapacity)
{
	p_vector->Writer.Core_VectorWriter_UserObject = p_vector;
	p_vector->Writer.PushBack = Core_Vector_pushBack;

	p_vector->Capacity = p_initialCapacity;
	p_vector->Memory = malloc(Core_Vector_getTotalSize(p_vector));
	p_vector->Size = 0;
};

void Core_Vector_free(Core_Vector* p_vector)
{
	free(p_vector->Memory);
	p_vector->Memory = NULL;
	p_vector->Capacity = 0;
	p_vector->ElementSize = 0;
	p_vector->Size = 0;
};