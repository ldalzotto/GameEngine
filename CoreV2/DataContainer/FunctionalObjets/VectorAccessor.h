#pragma message (__FILE__)

typedef struct Core_VectorAccessor
{
	void* Core_VectorAccessor_DataStructure;
	void* (*At)(void* p_dataStructure, size_t p_index);
	void* (*At_unchecked)(void* p_dataStructure, size_t p_index);
} Core_VectorAccessor;