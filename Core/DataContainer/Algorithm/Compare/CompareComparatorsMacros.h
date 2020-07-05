#pragma once

#define CORE_ALGORITHM_COMPARE_COMPARATOR_TYPE(DataContainerName, ComparedObjectTypeName, UserObjectTypeName) DataContainerName##_##ComparedObjectTypeName##_##UserObjectTypeName##_CompareElementComparatorWithUserObject

#define CORE_ALGORITHM_COMPARE_COMPARATOR(DataContainerName, ElementTypeName, ComparedObjectTypeName, UserObjectTypeName) \
	typedef bool(*CORE_ALGORITHM_COMPARE_COMPARATOR_TYPE(DataContainerName, ComparedObjectTypeName, UserObjectTypeName))(ElementTypeName*, ComparedObjectTypeName*, UserObjectTypeName*);