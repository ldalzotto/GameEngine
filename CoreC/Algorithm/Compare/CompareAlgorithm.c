#include "CompareAlgorithm.h"

#include <stdlib.h>

void* Core_find(struct Core_VectorIterator* p_iterator, struct Core_Comparator* p_comparator)
{
	while (p_iterator->Functions->MoveNext(p_iterator))
	{
		if (p_comparator->Function(p_iterator->Current, p_comparator->ComparedObject, p_comparator->UserObject))
		{
			return p_iterator->Current;
		}
	}

	return NULL;
};