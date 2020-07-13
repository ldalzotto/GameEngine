#pragma once

namespace _Core
{
	struct VectorIterator;
	struct Comparator;
}

namespace _Core
{
	bool Compare_find(VectorIterator* p_iterator, Comparator* p_comparator);
	bool Compare_contains(VectorIterator* p_iterator, Comparator* p_comparator);
}