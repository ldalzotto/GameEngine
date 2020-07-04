#pragma once

#define CORE_VECTORITERATOR_TYPE(DataStructureTypeName) DataStructureTypeName##Iterator
#define CORE_VECTORITERATOR_TYPE_FROMELEMENT(ElementTypeName, DataStructureTypeMacroProvider) DataStructureTypeMacroProvider(ElementTypeName)##Iterator

// #define CORE_VECTORITERATOR_TYPE(ElementTypeName) Core_##ElementTypeName##_VectorIterator

#define CORE_VECTORITERATOR_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORITERATOR_TYPE(##DataStructureTypeName) \
	{ \
		##DataStructureTypeName* Core_VectorIterator_DataStructure; \
		##ElementTypeName* Current; \
		size_t CurrentIndex; \
		##ElementTypeName* (*At)(DataStructureTypeName* p_userObject, size_t p_index); \
		##ElementTypeName* (*At_unchecked)(DataStructureTypeName* p_dataStructure, size_t p_index); \
	} CORE_VECTORITERATOR_TYPE(##DataStructureTypeName);

#define CORE_VECTORITERATOR_BUILD(DataStructureTypeName, in_DataStructure, var_VectorIterator) \
	CORE_VECTORITERATOR_TYPE(DataStructureTypeName) var_VectorIterator; \
	(in_DataStructure)->BuildIterator(##in_DataStructure, &##var_VectorIterator);

#define CORE_VECTORITERATOR_FOREACH_BEGIN(DataStructureTypeName, in_DataStructure, var_VectorIterator) \
	{ \
		CORE_VECTORITERATOR_BUILD(DataStructureTypeName, in_DataStructure, var_VectorIterator) \
		while(Core_VectorIterator_moveNext((Core_VectorIterator*)&var_VectorIterator)) \
		{

#define CORE_VECTORITERATOR_FOREACH_END() \
		} \
	}
