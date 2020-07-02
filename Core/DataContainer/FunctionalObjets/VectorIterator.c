#include "VectorIterator.h"

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator)
{
	p_iterator->CurrentIndex += 1;
	p_iterator->Current = p_iterator->GetElement(p_iterator->Core_VectorIterator_UserObject, p_iterator->CurrentIndex);
	return p_iterator->Current != NULL;
};