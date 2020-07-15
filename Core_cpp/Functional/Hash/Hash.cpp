#include "Hash.hpp"
#include <string.h>

namespace _Core
{

	// http://www.cse.yorku.ca/~oz/hash.html
	size_t Hash(void* p_value, size_t p_size)
	{
		size_t hash = 5381;

		for (size_t i = 0; i < p_size; i++)
		{
			char c = *((char*)p_value + i);
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}

		return hash;
	}

	size_t Hash_string(char* p_str)
	{
		return Hash(p_str, strlen(p_str));
	};
	
	size_t HashCombine_string(size_t p_seed, char* p_str)
	{
		return p_seed ^ (Hash_string(p_str) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
	};

	size_t Hash_uint(unsigned int* p_value)
	{
		return Hash(p_value, sizeof(unsigned int));
	};
	
	size_t HashCombine_uint(size_t p_seed, unsigned int* p_value)
	{
		return p_seed ^ (Hash_uint(p_value) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
	};
}
