#include "VectorIterator.h"

#include "DataStructures/GenericArray.h"

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_vectorIterator)
{
	p_vectorIterator->CurrentIndex += 1;
	p_vectorIterator->Current = Core_GenericArray_at(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
	return p_vectorIterator->Current != NULL;
};
