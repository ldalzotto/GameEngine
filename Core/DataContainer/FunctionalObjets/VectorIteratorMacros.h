#pragma once

#define CORE_VECTORITERATOR_TYPE(ElementTypeName) Core_##ElementTypeName##_VectorIterator

#define CORE_VECTORITERATOR_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORITERATOR_TYPE(##ElementTypeName) \
	{ \
		##DataStructureTypeName* Core_VectorIterator_DataStructure; \
		##ElementTypeName* Current; \
		size_t CurrentIndex; \
		##ElementTypeName* (*At)(DataStructureTypeName* p_userObject, size_t p_index); \
		##ElementTypeName* (*At_unchecked)(DataStructureTypeName* p_dataStructure, size_t p_index); \
	} CORE_VECTORITERATOR_TYPE(##ElementTypeName);

#define CORE_VECTORITERATOR_BUILD(ElementTypeName, in_DataStructure, var_VectorIterator) \
	CORE_VECTORITERATOR_TYPE(ElementTypeName) var_VectorIterator; \
	Core_Vector_buildIterator((Core_Vector*)##in_DataStructure, (Core_VectorIterator*)&##var_VectorIterator);

#define CORE_VECTORITERATOR_FOREACH_BEGIN(ElementTypeName, in_DataStructure, var_VectorIterator) \
	{ \
		CORE_VECTORITERATOR_BUILD(ElementTypeName, in_DataStructure, var_VectorIterator) \
		while(Core_VectorIterator_moveNext((Core_VectorIterator*)&var_VectorIterator)) \
		{

#define CORE_VECTORITERATOR_FOREACH_END() \
		} \
	}
