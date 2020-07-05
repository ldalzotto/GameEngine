#include "VectorIterator.h"
#include "DataContainer/FunctionalObjets/VectorAccessor.h"

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator, struct Core_VectorAccessor* p_accessor)
{
	p_iterator->CurrentIndex += 1;
	p_iterator->Current = p_accessor->At(p_iterator->Core_VectorIterator_DataStructure, p_iterator->CurrentIndex);
	return p_iterator->Current != NULL;
};
