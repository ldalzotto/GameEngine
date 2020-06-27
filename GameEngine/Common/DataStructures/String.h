#pragma once

#include "VectorT.h"

namespace _GameEngine::_Core
{
	struct String
	{
		_Core::VectorT<char> Vector;
		void alloc(size_t p_charNb);
		void free();
		void append(char* p_appended);
		void reserve(size_t p_charNb);
		char* c_str();
	};
}