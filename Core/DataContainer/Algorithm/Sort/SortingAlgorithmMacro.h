
#include "SortComparatorsMacros.h"

// Type safe interface

#define CORE_ALGORITHM_METHOD_NAME(DataContainerName, MethodName) DataContainerName##_##MethodName

#define CORE_SORT_ALGORITHM_DEFINE(DataContainerName, ElementTypeName) \
	__forceinline ElementTypeName* CORE_ALGORITHM_METHOD_NAME(DataContainerName, min)(struct CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject); \
	__forceinline size_t CORE_ALGORITHM_METHOD_NAME(DataContainerName, minIndex)(struct CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, size_t p_beginIndex, void* p_userObject); \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, selectionSort)(struct CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, struct CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject);

#define CORE_SORT_ALGORITHM_DEFINE_IMPL(DataContainerName, ElementTypeName) \
	__forceinline ElementTypeName* CORE_ALGORITHM_METHOD_NAME(DataContainerName, min)(CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject) \
	{ \
		return (##ElementTypeName*) Core_min((Core_VectorIterator*) p_iterator, (Core_SortElementComparatorWithUserObject)p_sortComparator, p_userObject); \
	}; \
	\
	__forceinline size_t CORE_ALGORITHM_METHOD_NAME(DataContainerName, minIndex)(CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, size_t p_beginIndex, void* p_userObject) \
	{ \
		return Core_minIndex((Core_VectorIterator*) p_iterator, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_beginIndex, p_userObject); \
	}; \
	__forceinline void CORE_ALGORITHM_METHOD_NAME(DataContainerName, selectionSort)(CORE_VECTORITERATOR_TYPE(##DataContainerName)* p_iterator, CORE_VECTORWRITER_TYPE(##DataContainerName)* p_writer, CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName) p_sortComparator, void* p_userObject) \
	{ \
		Core_selectionSort((Core_VectorIterator*) p_iterator, (Core_VectorWriter*) p_writer, (Core_SortElementComparatorWithUserObject) p_sortComparator, p_userObject); \
	};


#define CORE_ALGO_MIN(DataContainerName, ElementTypeName, var_minReturnValue, SortCompareFn, VectorIterator) \
	ElementTypeName* ##var_minReturnValue = CORE_ALGORITHM_METHOD_NAME(##DataContainerName, min)(##VectorIterator, ##SortCompareFn, NULL);

#define CORE_ALGO_MIN_IT(DataContainerName, ElementTypeName, in_DataStructure, var_minReturnValue, SortCompareFn) \
	ElementTypeName* ##var_minReturnValue = NULL; \
	{ \
		CORE_VECTORITERATOR_BUILD(##DataContainerName, ##in_DataStructure, l_core_algo_min_it_macro_iterator); \
		##var_minReturnValue = CORE_ALGORITHM_METHOD_NAME(##DataContainerName, min)(&l_core_algo_min_it_macro_iterator, ##SortCompareFn, NULL); \
	}

#define CORE_ALGO_SELECTION_SORT(DataContainerName, in_out_DataStructure, SortCompareFn, VectorIterator, VectorWriter) \
	CORE_ALGORITHM_METHOD_NAME(##DataContainerName, selectionSort)(##VectorIterator, ##VectorWriter, ##SortCompareFn, NULL);

#define CORE_ALGO_SELECTION_SORT_IT(DataContainerName, ElementTypeName, in_out_DataStructure, SortCompareFn) \
	{ \
		CORE_VECTORITERATOR_BUILD(##DataContainerName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_iterator); \
		CORE_VECTORWRITER_BUILD(##DataContainerName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_writer); \
		CORE_ALGO_SELECTION_SORT(##DataContainerName, ##in_out_DataStructure, ##SortCompareFn, &l_core_algo_selection_sort_it_macro_iterator, l_core_algo_selection_sort_it_macro_writer); \
	}
