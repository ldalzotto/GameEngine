#pragma once

#include <stdlib.h>

#define STRINGIFY(T1) #T1
#define STRINGIFY_CONCAT_TOKENS(T1, T2, T3) STRINGIFY(T1##T2##T3) 

#define CORE_SORTEDVECTOR_TYPE(ElementTypeName) Core_##ElementTypeName##_SortedVector

#define CORE_DEFINE_SORTEDVECTOR(ElementTypeName) \
\
typedef short int(*CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE))(##ElementTypeName*, ##ElementTypeName*, void*); \
\
typedef struct CORE_SORTEDVECTOR_TYPE(##ElementTypeName) \
{ \
	##ElementTypeName* Memory; \
	size_t Size; \
	size_t Capacity; \
	size_t ElementSize; \
	\
	CORE_VECTORACCESSOR_TYPE_FROMELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) Accessor; \
	CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) Writer; \
	void (*BuildIterator)(struct CORE_SORTEDVECTOR_TYPE(##ElementTypeName)* p_vector, struct CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE)* out_iterator); \
	CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) SortComparator; \
} CORE_SORTEDVECTOR_TYPE(##ElementTypeName); \
\
__forceinline void CORE_SORTEDVECTOR_TYPE(ElementTypeName)##_alloc(CORE_SORTEDVECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity, CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) p_sortComparator); \
__forceinline void CORE_SORTEDVECTOR_TYPE(ElementTypeName)##_free(CORE_SORTEDVECTOR_TYPE(ElementTypeName)* p_vector);

#define CORE_DEFINE_SORTEDVECTOR_IMPL(ElementTypeName) \
__forceinline void CORE_SORTEDVECTOR_TYPE(ElementTypeName)##_alloc(CORE_SORTEDVECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity, CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_SORTEDVECTOR_TYPE) p_sortComparator) \
{ \
	Core_SortedVector_alloc((Core_SortedVector*)p_vector, sizeof(ElementTypeName), p_initialCapacity, (Core_SortElementComparatorWithUserObject)p_sortComparator); \
}; \
__forceinline void CORE_SORTEDVECTOR_TYPE(ElementTypeName)##_free(CORE_SORTEDVECTOR_TYPE(ElementTypeName)* p_vector) \
{ \
	Core_SortedVector_free((Core_SortedVector*)p_vector); \
};
