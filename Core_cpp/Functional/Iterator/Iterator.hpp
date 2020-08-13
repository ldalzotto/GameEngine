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
		char* Current;
		size_t CurrentIndex;
	};

	bool VectorIterator_moveNext(VectorIterator* p_vectorIterator);
	bool VectorIterator_moveToIndex(VectorIterator* p_vectorIterator, size_t p_index);

	struct VectorReverseIterator
	{
		GenericArray* Array;
		char* Current;
		size_t CurrentIndex;
	};

	bool VectorReverseIterator_moveNext(VectorIterator* p_vectorIterator);

	struct VectorRangedIterator
	{
		GenericArray* Array;
		char* Current;
		size_t CurrentIndex;
		size_t BeginInluded;
		size_t EndExcluded;
	};

	bool VectorRangedIterator_moveNext(VectorRangedIterator* p_vectorIterator);
}