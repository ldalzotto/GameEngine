#pragma once

#include "SortComparators.h"
#include <stdlib.h>

void* Core_min(struct Core_VectorAccessor* p_accessor, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject);
size_t Core_minIndex(struct Core_VectorAccessor* p_accessor, Core_SortElementComparatorWithUserObject p_sortComparator, size_t p_beginIndex, void* p_userObject);
void Core_selectionSort(struct Core_VectorAccessor* p_accessor, struct Core_VectorWriter* p_writer, Core_SortElementComparatorWithUserObject p_sortComparator, void* p_userObject);
