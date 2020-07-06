#pragma message (__FILE__)

#define CORE_VECTORACCESSOR_TYPE(DataStructureTypeName) DataStructureTypeName##Accessor
#define CORE_VECTORACCESSOR_TYPE_FROMELEMENT(ElementTypeName, DataStructureTypeMacroProvider) DataStructureTypeMacroProvider(ElementTypeName)##Accessor

#define CORE_VECTORACCESSOR_DEFINE(DataStructureTypeName, ElementTypeName) \
	typedef struct CORE_VECTORACCESSOR_TYPE(##DataStructureTypeName) \
	{ \
		struct DataStructureTypeName* Core_VectorAccessor_DataStructure; \
		##ElementTypeName* (*At)(struct DataStructureTypeName* p_dataStructure, size_t p_index); \
		##ElementTypeName* (*At_unchecked)(struct DataStructureTypeName* p_dataStructure, size_t p_index); \
	} CORE_VECTORACCESSOR_TYPE(##DataStructureTypeName);
