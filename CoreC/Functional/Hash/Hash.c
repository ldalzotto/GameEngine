#include "Hash.h"

#include <string.h>

size_t Core_Hash(void* p_value, size_t p_size)
{
	size_t hash = 5381;

	for (size_t i = 0; i < p_size; i++)
	{
		char c = *((char*)p_value + i);
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

size_t Core_Hash_string(char* p_str)
{
	return Core_Hash(p_str, strlen(p_str));
}

size_t Core_HashCombine_string(size_t p_seed, char* p_str)
{
	return p_seed ^ (Core_Hash_string(p_str) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
};


size_t Core_Hash_uint(unsigned int* p_value)
{
	return Core_Hash(p_value, sizeof(unsigned int));
};

size_t Core_HashCombine_uint(size_t p_seed, unsigned int* p_value)
{
	return p_seed ^ (Core_Hash_uint(p_value) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
};