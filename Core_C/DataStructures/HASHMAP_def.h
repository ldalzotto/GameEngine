#pragma once

typedef size_t(*HashMap_HashFn)(char* p_key);

typedef struct HashMap_TYP
{
	char* Entries;
	size_t Capacity;
	HashMap_HashFn HashFn;
}HashMap, * HashMap_PTR;

