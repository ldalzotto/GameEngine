#pragma once

typedef bool(*Core_comparator_function)(void* p_sourceObject, void* p_comparedObject, void* p_userObject);

typedef struct Core_Comparator
{
	Core_comparator_function Function;
	void* ComparedObject;
	void* UserObject;

} Core_Comparator;

