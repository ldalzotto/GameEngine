#pragma once

namespace _Core
{
	struct VectorIterator;
	struct ElementSorter;
}

namespace _Core
{
	void Sort_min(VectorIterator* p_vectorIterator, size_t p_startIndex, ElementSorter* p_sortComparator);
	void Sort_selectionSort(VectorIterator* p_vectorIterator, ElementSorter* p_sortComparator);
}