#pragma once

#include <stdlib.h>
#include "VectorT.h"
#include "ArrayT.h"

namespace _GameEngine::_Core
{
	struct String
	{
		_Core::VectorT<char> Vector;
		void alloc(size_t p_charNb);
		void free();
		void reserve(size_t p_charNb);
		char* c_str();
	};

	/**
	//TODO This method is temporary and must be replaced with a more generic string modification system (iterator, writer, ...)
	*/
	String String_interpret(char* p_source, size_t p_sourceCharNb);

	struct FixedString
	{
		_Core::ArrayT<char> Array;
		char* c_str();
	};

	FixedString FixedString_interpret(char* p_source);
	FixedString FixedString_interpret(char* p_source, size_t p_sourceCharNb);

	inline size_t String_charNb(_Core::String* p_string)
	{
		return p_string->Vector.size() - 1;
	};

	inline size_t String_charNb(_Core::FixedString* p_string)
	{
		return p_string->Array.size() - 1;
	};

}