#pragma message (__FILE__)

#define CORE_ARRAY_TYPE(ElementTypeName) Core_##ElementTypeName##_Array
#define CORE_ARRAY_RAW_NBELEM(x)  (sizeof(x) / sizeof((x)[0]))

#define CORE_DEFINE_ARRAY(ElementTypeName) \
typedef struct CORE_ARRAY_TYPE(ElementTypeName) \
{ \
	ElementTypeName* Memory; \
	size_t Size; \
	size_t Capacity; \
	size_t ElementSize; \
	\
	CORE_VECTORACCESSOR_TYPE_FROMELEMENT(ElementTypeName, CORE_ARRAY_TYPE) Accessor; \
	CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, CORE_ARRAY_TYPE) Writer; \
	void (*BuildIterator)(struct CORE_ARRAY_TYPE(ElementTypeName)* p_vector, struct CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, CORE_ARRAY_TYPE)* out_iterator); \
} CORE_ARRAY_TYPE(ElementTypeName); \
\
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_alloc(CORE_ARRAY_TYPE(ElementTypeName)* p_array, size_t p_initialCapacity); \
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_free(CORE_ARRAY_TYPE(ElementTypeName)* p_array); \
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_fromRawArray(CORE_ARRAY_TYPE(ElementTypeName)* p_array, ElementTypeName* p_rawArray, size_t p_rawArrayElementNb);


#define CORE_DEFINE_ARRAY_IMPL(ElementTypeName) \
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_alloc(CORE_ARRAY_TYPE(ElementTypeName)* p_array, size_t p_initialCapacity) \
{ \
	Core_Array_alloc((Core_Array*)p_array, sizeof(ElementTypeName), p_initialCapacity); \
}; \
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_free(CORE_ARRAY_TYPE(ElementTypeName)* p_array) \
{ \
	Core_Array_free((Core_Array*)p_array); \
}; \
__forceinline void CORE_ARRAY_TYPE(ElementTypeName)##_fromRawArray(CORE_ARRAY_TYPE(ElementTypeName)* p_array, ElementTypeName* p_rawArray, size_t p_rawArrayElementNb) \
{ \
	Core_Array_fromRawArray((Core_Array*)p_array, (void*) p_rawArray, sizeof(ElementTypeName), p_rawArrayElementNb); \
};
