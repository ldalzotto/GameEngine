#pragma once

#include <stdlib.h>

#include "DataStructures/GenericArray.h"
#include "Functional/Sort/ElementSorter.h"

void* Core_min(struct Core_GenericArray* p_genericArray, struct Core_ElementSorter* p_sortComparator);
size_t Core_minIndex(struct Core_GenericArray* p_genericArray, size_t p_beginIndex, struct Core_ElementSorter* p_sortComparator);
void Core_selectionSort(struct Core_GenericArray* p_genericArray, struct Core_ElementSorter* p_sortComparator);