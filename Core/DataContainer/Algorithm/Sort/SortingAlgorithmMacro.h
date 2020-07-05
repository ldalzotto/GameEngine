
#include "SortComparatorsMacros.h"

// Type safe interface

#define CORE_ALGORITHM_METHOD_NAME(DataContainerName, MethodName) DataContainerName##_##MethodName

#define CORE_SORT_ALGORITHM_DEFINE(DataContainerName, ElementTypeName) \
	__forceinline ElementTypeName* CORE_ALGORITHM_METHOD_NAME(DataContainerName, min)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject); \
	__forceinline size_t CORE_ALGORITHM_METHOD_NAME(DataContainerName, minIndex)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, size_t p_beginIndex, void* p_userObject); \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, selectionSort)(struct CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, struct CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject);

#define CORE_SORT_ALGORITHM_DEFINE_IMPL(DataContainerName, ElementTypeName) \
	__forceinline ElementTypeName* CORE_ALGORITHM_METHOD_NAME(DataContainerName, min)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject) \
	{ \
		return (##ElementTypeName*) Core_min((Core_VectorAccessor*) p_accessor, (Core_SortElementComparatorWithUserObject)p_sortComparator, p_userObject); \
	}; \
	\
	__forceinline size_t CORE_ALGORITHM_METHOD_NAME(DataContainerName, minIndex)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, size_t p_beginIndex, void* p_userObject) \
	{ \
		return Core_minIndex((Core_VectorAccessor*) p_accessor, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_beginIndex, p_userObject); \
	}; \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, selectionSort)(CORE_VECTORACCESSOR_TYPE(##DataContainerName)* p_accessor, CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject) \
	{ \
		Core_selectionSort((Core_VectorAccessor*) p_accessor, (Core_VectorWriter*) p_writer, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_userObject); \
	};


#define CORE_ALGO_MIN(DataContainerName, ElementTypeName, var_minReturnValue, SortCompareFn, VectorIterator) \
	ElementTypeName* ##var_minReturnValue = CORE_ALGORITHM_METHOD_NAME(##DataContainerName, min)(##VectorIterator, ##SortCompareFn, NULL);

#define CORE_ALGO_MIN_IT(DataContainerName, ElementTypeName, in_DataStructure, var_minReturnValue, SortCompareFn) \
	ElementTypeName* ##var_minReturnValue = NULL; \
	{ \
		##var_minReturnValue = CORE_ALGORITHM_METHOD_NAME(##DataContainerName, min)((CORE_VECTORACCESSOR_TYPE(##DataContainerName)*)(&(##in_DataStructure)->Accessor), ##SortCompareFn, NULL); \
	}

#define CORE_ALGO_SELECTION_SORT(DataContainerName, in_out_DataStructure, SortCompareFn, VectorAccessor, VectorWriter) \
	CORE_ALGORITHM_METHOD_NAME(##DataContainerName, selectionSort)(##VectorAccessor, ##VectorWriter, ##SortCompareFn, NULL);

#define CORE_ALGO_SELECTION_SORT_IT(DataContainerName, ElementTypeName, in_out_DataStructure, SortCompareFn) \
	{ \
		CORE_VECTORWRITER_BUILD(##DataContainerName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_writer); \
		CORE_ALGO_SELECTION_SORT(##DataContainerName, ##in_out_DataStructure, ##SortCompareFn, &(in_out_DataStructure)->Accessor, l_core_algo_selection_sort_it_macro_writer); \
	}
