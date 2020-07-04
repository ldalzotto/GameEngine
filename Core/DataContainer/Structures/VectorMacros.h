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
} CORE_VECTOR_TYPE(ElementTypeName);
