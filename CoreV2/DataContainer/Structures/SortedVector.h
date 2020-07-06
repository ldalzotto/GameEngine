#pragma message (__FILE__)

typedef struct Core_SortedVector
{
	Core_GenericArray GenericArray;
	Core_VectorAccessor Accessor;
	Core_VectorWriter Writer;
	void (*BuildIterator)(struct Core_SortedVector* p_vector, struct Core_VectorIterator* out_iterator);
	Core_SortElementComparatorWithUserObject SortComparator;
} Core_SortedVector;

void Core_SortedVector_alloc(Core_SortedVector* p_vector, size_t p_elementSize, size_t p_initialCapacity, Core_SortElementComparatorWithUserObject p_sortComparator);
void Core_SortedVector_free(Core_SortedVector* p_vector);