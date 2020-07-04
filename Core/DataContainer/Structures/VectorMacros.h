#pragma once

#include <stdlib.h>

#include "DataContainer/FunctionalObjets/VectorWriterMacros.h"

#define CORE_VECTOR_TYPE(ElementTypeName) Core_##ElementTypeName##_Vector

#define CORE_DEFINE_VECTOR(ElementTypeName) \
typedef struct CORE_VECTOR_TYPE(ElementTypeName) \
{ \
	ElementTypeName* Memory = NULL; \
	size_t Size = 0; \
	size_t Capacity = 0; \
	size_t ElementSize = sizeof(ElementTypeName); \
	\
	CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, CORE_VECTOR_TYPE) Writer; \
	void (*BuildIterator)(struct CORE_VECTOR_TYPE(ElementTypeName)* p_vector, struct CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, CORE_VECTOR_TYPE)* out_iterator); \
} CORE_VECTOR_TYPE(ElementTypeName); \
\
typedef int(*CORE_ALGORITHM_SORT_COMPARATOR_TYPE_FROM_ELEMENT(ElementTypeName, CORE_VECTOR_TYPE))(##ElementTypeName*, ##ElementTypeName*, void*); \
\
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_alloc(CORE_VECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity); \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_free(CORE_VECTOR_TYPE(ElementTypeName)* p_vector);


#define CORE_DEFINE_VECTOR_IMPL(ElementTypeName) \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_alloc(CORE_VECTOR_TYPE(ElementTypeName)* p_vector, size_t p_initialCapacity) \
{ \
	Core_Vector_alloc((Core_Vector*)p_vector, p_initialCapacity); \
}; \
__forceinline void CORE_VECTOR_TYPE(ElementTypeName)##_free(CORE_VECTOR_TYPE(ElementTypeName)* p_vector) \
{ \
	Core_Vector_free((Core_Vector*)p_vector); \
};
