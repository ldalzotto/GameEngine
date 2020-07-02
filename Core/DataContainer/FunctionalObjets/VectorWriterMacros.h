#pragma once

#define CORE_VECTORWRITER_TYPE(ElementTypeName) Core_##ElementTypeName##_VectorWriter

#define CORE_VECTORWRITER_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORWRITER_TYPE(##ElementTypeName) \
	{ \
		struct DataStructureTypeName* Core_VectorWriter_UserObject; \
		void (*PushBack)(##DataStructureTypeName* p_userObject, ##ElementTypeName* p_value); \
	} CORE_VECTORWRITER_TYPE(##ElementTypeName); \
	\
	void CORE_VECTORWRITER_TYPE(##ElementTypeName)_pushBack(CORE_VECTORWRITER_TYPE(##ElementTypeName)* p_writer, ##ElementTypeName* p_value);

#define CORE_VECTORWRITER_DEFINE_IMPL(DataStructureTypeName, ElementTypeName) \
	void CORE_VECTORWRITER_TYPE(##ElementTypeName)_pushBack(CORE_VECTORWRITER_TYPE(##ElementTypeName)* p_writer, ##ElementTypeName* p_value) \
	{ \
	    p_writer->PushBack(p_writer->Core_VectorWriter_UserObject, p_value); \
	}