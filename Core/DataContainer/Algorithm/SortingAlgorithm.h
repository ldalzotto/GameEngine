#pragma once

typedef int(*Core_SortElementComparatorWithUserObject)(void*, void*, void*);

void* Core_min(struct Core_VectorIterator* p_iterator, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject);