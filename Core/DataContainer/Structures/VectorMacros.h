#pragma once

#include <stdlib.h>

//#include "DataContainer/FunctionalObjets/VectorWriter.h"

#define STRINGIFY(T1) #T1
#define STRINGIFY_CONCAT_TOKENS(T1, T2, T3) STRINGIFY(T1##T2##T3) 

#define CORE_VECTOR_TYPE(ElementTypeName) Core_##ElementTypeName##_Vector

#define CORE_DEFINE_VECTOR(ElementTypeName) \
typedef struct CORE_VECTOR_TYPE(##ElementTypeName) \
{ \
	##ElementTypeName* Memory = NULL; \
	size_t Size = 0; \
	size_t Capacity = 0; \
	size_t ElementSize = sizeof(##ElementTypeName); \
	\
	CORE_VECTORWRITER_TYPE(##ElementTypeName) Writer; \
} CORE_VECTOR_TYPE(##ElementTypeName);
