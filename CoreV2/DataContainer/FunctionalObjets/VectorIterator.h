#pragma message (__FILE__)

typedef struct Core_VectorIterator
{
	void* Core_VectorIterator_DataStructure;
	void* Current;
	size_t CurrentIndex;
} Core_VectorIterator;

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator, struct Core_VectorAccessor* p_accessor);

