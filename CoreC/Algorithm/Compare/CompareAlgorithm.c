#include "CompareAlgorithm.h"

#include <stdlib.h>

#include "Functional/Vector/VectorIterator.h"
#include "Functional/Comparator/Comparator.h"

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