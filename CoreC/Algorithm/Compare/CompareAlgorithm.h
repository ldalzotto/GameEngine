#pragma once

#include <stdbool.h>

void* Core_find(struct Core_VectorIterator* p_iterator, struct Core_Comparator* p_comparator);
bool Core_contains(struct Core_VectorIterator* p_iterator, struct Core_Comparator* p_comparator);