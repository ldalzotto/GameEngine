#include "AllocationDebug.h"

#include "DataStructures/STRING.h"
#include "Functional/ToString/ToString.h"

#define ALLOCATIONDEBUG_LINEMAXSIZE 500

FileStream AllocationDebugOutput = { 0 };

void AllocationDebug_Alloc()
{
	AllocationDebugOutput = FileStream_open("E:/GameProjects/GameEngine/out/DebugMemoty.txt", FILESTREAM_MODE_WRITE);
};

void AllocationDebug_Write(void* p_involvedMemory, ALLOCATIONDEBUG_TYPE p_allocationType, const char* p_sourceFile, const int p_line)
{
	char l_buffer[ALLOCATIONDEBUG_LINEMAXSIZE];
	String l_line = { .Memory = l_buffer, .Size = 0, .Capacity = ALLOCATIONDEBUG_LINEMAXSIZE };
	String_ClearNoRealloc(&l_line);
	String_AppendRawNoRealloc(&l_line, p_sourceFile);
	String_AppendRawNoRealloc(&l_line, ":");

	{
		char l_lineStr[20];
		ToString_intv(&l_lineStr, &p_line);
		String_AppendRawNoRealloc(&l_line, l_lineStr);
	}

	FileStream_writeSyncRaw(&AllocationDebugOutput.Stream, l_line.Memory, l_line.Size);
};

void AllocationDebug_Free()
{
	FileStream_close(&AllocationDebugOutput);
	AllocationDebugOutput = (FileStream){ 0 };
};