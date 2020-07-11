#pragma once

typedef struct Core_VectorWriter
{
	void (*Clear)(struct Core_GenericArray* p_dataStructure);
	Core_ReturnCodes(*PushBack)(struct Core_GenericArray* p_dataStructure, void* p_value);
	Core_ReturnCodes(*Swap)(struct Core_GenericArray* p_dataStructure, size_t p_left, size_t p_right);
	Core_ReturnCodes(*InsertAt)(struct Core_GenericArray* p_dataStructure, void* p_insertedElement, size_t p_elementNb, size_t p_index);
	Core_ReturnCodes(*InsertArrayAt)(struct Core_GenericArray* p_dataStructure, void* p_insertedDataStructure, size_t p_index);
	Core_ReturnCodes(*EraseAt)(struct Core_GenericArray* p_dataStructure, size_t p_index);
	void(*EraseCompare)(struct Core_GenericArray* p_dataStructure, struct Core_Comparator* p_comparator);
} Core_VectorWriter;