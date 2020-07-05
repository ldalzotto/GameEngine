#pragma once

#define CORE_ALGORITHM_METHOD_NAME(DataContainerName, MethodName) DataContainerName##_##MethodName

#define CORE_STRING_ALGORITHM_DEFINE(DataContainerName) \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, append)(struct DataContainerName* p_string, struct CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, char* p_appended); \
	__forceinline bool CORE_ALGORITHM_METHOD_NAME(DataContainerName, find)(struct DataContainerName* p_string, struct CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);

#define CORE_STRING_ALGORITHM_DEFINE_IMPL(DataContainerName) \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, append)(DataContainerName* p_string, CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, char* p_appended) \
	{ \
		Core_string_append((Core_GenericArray*)p_string, (Core_VectorWriter*)p_writer, p_appended); \
	}; \
	\
	__forceinline bool CORE_ALGORITHM_METHOD_NAME(DataContainerName, find)(DataContainerName* p_string, CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex) \
	{ \
		return Core_string_find((Core_GenericArray*) p_string, (Core_VectorIterator*)p_iterator, p_comparedStr, p_startIndex, p_outfoundIndex); \
	};

// void Core_string_append(struct Core_GenericArray* p_string, struct Core_VectorWriter* p_writer, char* p_appended);
// bool Core_string_find(struct Core_GenericArray* p_string, struct Core_VectorIterator* p_iterator, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);