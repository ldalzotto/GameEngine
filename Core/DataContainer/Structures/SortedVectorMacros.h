#pragma once

#include <stdlib.h>

#define STRINGIFY(T1) #T1
#define STRINGIFY_CONCAT_TOKENS(T1, T2, T3) STRINGIFY(T1##T2##T3) 

#define CORE_SORTEDVECTOR_TYPE(ElementTypeName) Core_##ElementTypeName##_SortedVector

#define CORE_DEFINE_SORTEDVECTOR(ElementTypeName) \
typedef struct CORE_SORTEDVECTOR_TYPE(##ElementTypeName) \
{ \
	##ElementTypeName* Memory = NULL; \
	size_t Size = 0; \
	size_t Capacity = 0; \
	size_t ElementSize = sizeof(##ElementTypeName); \
	\
	CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) Writer; \
	void (*BuildIterator)(struct CORE_SORTEDVECTOR_TYPE(##ElementTypeName)* p_vector, struct CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE)* out_iterator); \
	CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) SortComparator; \
} CORE_SORTEDVECTOR_TYPE(##ElementTypeName);
