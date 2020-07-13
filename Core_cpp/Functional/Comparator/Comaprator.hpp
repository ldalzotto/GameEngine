#pragma once

namespace _Core
{
	typedef bool(*Comparator_function)(void* p_sourceObject, void* p_comparedObject, void* p_userObject);

	struct Comparator
	{
		Comparator_function Function;
		void* ComparedObject;
		void* UserObject;
	};

}