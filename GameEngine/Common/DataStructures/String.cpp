#include "String.h"
#include <string.h> 

namespace _GameEngine::_Core
{
	void String::alloc(size_t p_charNb)
	{
		this->Vector.alloc(p_charNb + 1);
		char l_nul = NULL;
		this->Vector.push_back(&l_nul);
	};

	void String::free()
	{
		this->Vector.free();
	};

	void String::append(char* p_appended)
	{
		size_t l_appendSize = strlen(p_appended);
		if (this->Vector.size() >= 2)
		{
			this->Vector.insertAt(p_appended, l_appendSize, this->Vector.size() - 1);
		}
		else
		{
			this->Vector.insertAt(p_appended, l_appendSize, 0);
		}
		
	};

	char* String::c_str()
	{
		return (char*)this->Vector.Vector.Memory;
	};
}