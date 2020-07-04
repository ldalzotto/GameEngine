
#define CORE_ALGO_MIN(ElementTypeName, var_minReturnValue, SortCompareFn, VectorIterator) \
	ElementTypeName* ##var_minReturnValue = (##ElementTypeName*) Core_min((Core_VectorIterator*)##VectorIterator, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL);

#define CORE_ALGO_MIN_IT(ElementTypeName, in_DataStructure, var_minReturnValue, SortCompareFn) \
	ElementTypeName* ##var_minReturnValue = NULL; \
	{ \
		CORE_VECTORITERATOR_BUILD(##ElementTypeName, ##in_DataStructure, l_core_algo_min_it_macro_iterator); \
		##var_minReturnValue = (##ElementTypeName*) Core_min((Core_VectorIterator*)&l_core_algo_min_it_macro_iterator, (Core_SortElementComparatorWithUserObject)##SortCompareFn, NULL); \
	}
