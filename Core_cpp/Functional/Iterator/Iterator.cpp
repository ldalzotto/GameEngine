#include "Iterator.hpp"

#include "DataStructures/GenericArray.hpp"

namespace _Core
{
	bool VectorIterator_moveNext(VectorIterator* p_vectorIterator)
	{
		p_vectorIterator->CurrentIndex += 1;
		p_vectorIterator->Current = GenericArray_at(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
		return p_vectorIterator->Current != nullptr;
	};

	bool VectorIterator_moveToIndex(VectorIterator* p_vectorIterator, size_t p_index)
	{
		p_vectorIterator->CurrentIndex = p_index;
		p_vectorIterator->Current = GenericArray_at(p_vectorIterator->Array, p_vectorIterator->CurrentIndex);
		return p_vectorIterator->Current != nullptr;
	};
}