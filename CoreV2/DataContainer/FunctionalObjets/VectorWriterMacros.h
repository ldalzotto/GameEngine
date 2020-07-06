#pragma message (__FILE__)

#define CORE_VECTORWRITER_TYPE(DataStructureTypeName) DataStructureTypeName##Writer
#define CORE_VECTORWRITER_TYPE_FROMELEMENT(ElementTypeName, DataStructureTypeMacroProvider) DataStructureTypeMacroProvider(ElementTypeName)##Writer

#define CORE_VECTORWRITER_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORWRITER_TYPE(DataStructureTypeName) \
	{ \
		struct DataStructureTypeName* Core_VectorWriter_UserObject; \
		void (*Clear)(ElementTypeName* p_dataStructure); \
		Core_ReturnCodes (*PushBack)(struct DataStructureTypeName* p_dataStructure, ElementTypeName* p_value); \
		Core_ReturnCodes (*Swap)(struct DataStructureTypeName* p_dataStructure, size_t p_left, size_t p_right); \
		Core_ReturnCodes (*InsertAt)(struct DataStructureTypeName* p_dataStructure, ElementTypeName* p_insertedElement, size_t p_elementNb, size_t p_index); \
		Core_ReturnCodes (*InsertArrayAt)(struct DataStructureTypeName* p_dataStructure, struct DataStructureTypeName* p_insertedDataStructure, size_t p_index); \
	} CORE_VECTORWRITER_TYPE(DataStructureTypeName);

#define CORE_VECTORWRITER_CLEAR(in_out_dataStructure) (in_out_dataStructure)->Writer.Clear(in_out_dataStructure);

#define CORE_VECTORWRITER_PUSHBACK(in_out_dataStructure, in_element) \
	{ \
		CORE_HANDLE_ERROR_BEGIN(err) \
			err = (in_out_dataStructure)->Writer.PushBack(in_out_dataStructure, in_element); \
		CORE_HANDLE_ERROR_END(err); \
	}

#define CORE_VECTORWRITER_INSERT_AT(in_out_dataStructure, in_insertedElement, in_elementNb, in_index) \
	{ \
		CORE_HANDLE_ERROR_BEGIN(err) \
			err = (in_out_dataStructure)->Writer.InsertAt(in_out_dataStructure, in_insertedElement, in_elementNb, in_index); \
		CORE_HANDLE_ERROR_END(err); \
	}

#define CORE_VECTORWRITER_BUILD(DataContainerName, in_DataStructure, var_VectorWriter) \
	CORE_VECTORWRITER_TYPE(##DataContainerName)* ##var_VectorWriter =  (CORE_VECTORWRITER_TYPE(##DataContainerName)*) &((Core_Vector*)##in_DataStructure)->Writer;
