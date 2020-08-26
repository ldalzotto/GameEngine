#pragma once
#include <stdio.h>
#include <stdlib.h>

extern char RETURN_CODE;

#ifndef NEDBUG
#define HANDLE_ERR(FuncCall) \
{RETURN_CODE = FuncCall; if(RETURN_CODE) { printf("Critical error in file:%s, function:%s() and line:%d", __FILE__, __func__, __LINE__);  abort(); } }
#else
#define HANDLE_ERR(FuncCall) \
FuncCall
#endif // !NEDBUG

#define ABORT_ERROR(Message) \
printf(Message); abort();

#define HANDLE_ERR_SILENT(FuncCall) \
{RETURN_CODE = FuncCall; if(RETURN_CODE) {return RETURN_CODE;} }

#define BUFFER_OVERFLOW_INTERNAL_CONDITION (BUFFER_OVERFLOW_INTERNAL_CHECK == 1) && (NDEBUG == 0)

inline void HandleBufferOverflowMemoryWrite(const char* p_dist, const size_t p_size, const char* p_distBufferStart, const size_t p_distBufferSize)
{
	const char* l_distArrayEnd = p_distBufferStart + p_distBufferSize;
	const char* l_distWrittenBytLast = p_dist + p_size;
	if (l_distWrittenBytLast > l_distArrayEnd)
	{
		printf("CRITICAL - Trying to write outside of array allocated memory."); 
		abort();
	}
};
