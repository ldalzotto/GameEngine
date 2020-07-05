
#include "SortComparatorsMacros.h"

// Type safe interface

#define CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, MethodName) DataContainerName##_##UserObjectTypeName##_##MethodName

#define CORE_SORT_ALGORITHM_DEFINE(DataContainerName, ElementTypeName, UserObjectTypeName) \
	CORE_ALGORITHM_SORT_COMPARATOR_DEFINE(DataContainerName, ElementTypeName, UserObjectTypeName) \
	__forceinline ElementTypeName* CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, min)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, void* p_userObject); \
	__forceinline size_t CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, minIndex)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, size_t p_beginIndex, void* p_userObject); \
	__forceinline void CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, selectionSort)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, struct CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, void* p_userObject);

#define CORE_SORT_ALGORITHM_DEFINE_IMPL(DataContainerName, ElementTypeName, UserObjectTypeName) \
	__forceinline ElementTypeName* CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, min)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, void* p_userObject) \
	{ \
		return (##ElementTypeName*) Core_min((Core_VectorAccessor*) p_accessor, (Core_SortElementComparatorWithUserObject)p_sortComparator, p_userObject); \
	}; \
	\
	__forceinline size_t CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, minIndex)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, size_t p_beginIndex, void* p_userObject) \
	{ \
		return Core_minIndex((Core_VectorAccessor*) p_accessor, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_beginIndex, p_userObject); \
	}; \
	__forceinline void CORE_SORT_ALGORITHM_METHOD_NAME(DataContainerName, UserObjectTypeName, selectionSort)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) p_sortComparator, void* p_userObject) \
	{ \
		Core_selectionSort((Core_VectorAccessor*) p_accessor, (Core_VectorWriter*) p_writer, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_userObject); \
	};


#define CORE_ALGO_MIN(DataContainerName, ElementTypeName, UserObjectTypeName, var_minReturnValue, SortCompareFn, VectorIterator) \
	ElementTypeName* ##var_minReturnValue = CORE_SORT_ALGORITHM_METHOD_NAME(##DataContainerName, UserObjectTypeName, min)(##VectorIterator, ##SortCompareFn, NULL);

#define CORE_ALGO_MIN_IT(DataContainerName, ElementTypeName, UserObjectTypeName, in_DataStructure, in_userObject, SortCompareFn, var_minReturnValue) \
	ElementTypeName* ##var_minReturnValue = NULL; \
	{ \
		##var_minReturnValue = CORE_SORT_ALGORITHM_METHOD_NAME(##DataContainerName, UserObjectTypeName, min)((CORE_VECTORACCESSOR_TYPE(##DataContainerName)*)(&(##in_DataStructure)->Accessor), ##SortCompareFn, ##in_userObject); \
	}

#define CORE_ALGO_SELECTION_SORT(DataContainerName, UserObjectTypeName, in_out_DataStructure, in_userObject, SortCompareFn, VectorAccessor, VectorWriter) \
	CORE_SORT_ALGORITHM_METHOD_NAME(##DataContainerName, UserObjectTypeName, selectionSort)(##VectorAccessor, ##VectorWriter, ##SortCompareFn, ##in_userObject);

#define CORE_ALGO_SELECTION_SORT_IT(DataContainerName, ElementTypeName, UserObjectTypeName, in_out_DataStructure, in_userObject, SortCompareFn) \
	{ \
		CORE_VECTORWRITER_BUILD(##DataContainerName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_writer); \
		CORE_ALGO_SELECTION_SORT(##DataContainerName, UserObjectTypeName, ##in_out_DataStructure, ##in_userObject, ##SortCompareFn, &(in_out_DataStructure)->Accessor, l_core_algo_selection_sort_it_macro_writer); \
	}
