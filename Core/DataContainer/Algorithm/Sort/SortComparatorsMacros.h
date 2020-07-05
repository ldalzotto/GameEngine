#pragma once

#define CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName) DataContainerName##_##UserObjectTypeName##_SortElementComparatorWithUserObject

#define CORE_ALGORITHM_SORT_COMPARATOR_DEFINE(DataContainerName, ElementTypeName, UserObjectTypeName) \
	typedef short int(*CORE_ALGORITHM_SORT_COMPARATOR_TYPE(DataContainerName, UserObjectTypeName))(ElementTypeName*, ElementTypeName*, UserObjectTypeName*);
