
#define CORE_ALGO_MIN(ElementTypeName, var_minReturnValue, SortCompareFn, VectorIterator) \
	ElementTypeName* ##var_minReturnValue = (##ElementTypeName*) Core_min((Core_VectorIterator*)##VectorIterator, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL);

#define CORE_ALGO_MIN_IT(ElementTypeName, in_DataStructure, var_minReturnValue, SortCompareFn) \
	ElementTypeName* ##var_minReturnValue = NULL; \
	{ \
		CORE_VECTORITERATOR_BUILD(##ElementTypeName, ##in_DataStructure, l_core_algo_min_it_macro_iterator); \
		##var_minReturnValue = (##ElementTypeName*) Core_min((Core_VectorIterator*)&l_core_algo_min_it_macro_iterator, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL); \
	}

#define CORE_ALGO_SELECTION_SORT(in_out_DataStructure, SortCompareFn, VectorIterator, VectorWriter) \
	Core_selectionSort((Core_VectorIterator*)##VectorIterator, (Core_VectorWriter*)##VectorWriter, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL);

#define CORE_ALGO_SELECTION_SORT_IT(ElementTypeName, in_out_DataStructure, SortCompareFn) \
	{ \
		CORE_VECTORITERATOR_BUILD(##ElementTypeName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_iterator); \
		CORE_VECTORWRITER_BUILD(##ElementTypeName, ##in_out_DataStructure, l_core_algo_selection_sort_it_macro_writer); \
		Core_selectionSort((Core_VectorIterator*)&l_core_algo_selection_sort_it_macro_iterator, (Core_VectorWriter*)l_core_algo_selection_sort_it_macro_writer, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL); \
	}	
