#pragma once

namespace _Core
{
	struct GenericArray;
}

namespace _Core
{
	struct VectorIterator
	{
		GenericArray* Array;
		void* Current;
		size_t CurrentIndex;
	};

	bool VectorIterator_moveNext(VectorIterator* p_vectorIterator);
	bool VectorIterator_moveToIndex(VectorIterator* p_vectorIterator, size_t p_index);
}