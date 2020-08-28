#pragma once

#include "Read/File/File.h"

typedef char ALLOCATIONDEBUG_TYPE;
#define ALLOCATIONDEBUG_TYPE_ALLOC 0
#define ALLOCATIONDEBUG_TYPE_FREE 1

void AllocationDebug_Alloc();
void AllocationDebug_Write(void* p_involvedMemory, ALLOCATIONDEBUG_TYPE p_allocationType, const char* p_sourceFile, const int p_line);
void AllocationDebug_Free();