#pragma message (__FILE__)

typedef struct Core_VectorWriter
{
	void* Core_VectorWriter_UserObject;
	void (*Clear)(void* p_dataStructure);
	Core_ReturnCodes (*PushBack)(void* p_dataStructure, void* p_value);
	Core_ReturnCodes (*Swap)(void* p_dataStructure, size_t p_left, size_t p_right);
	Core_ReturnCodes (*InsertAt)(void* p_dataStructure, void* p_insertedElement, size_t p_elementNb, size_t p_index);
	Core_ReturnCodes (*InsertArrayAt)(void* p_dataStructure, void* p_insertedDataStructure, size_t p_index);
} Core_VectorWriter;