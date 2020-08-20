#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "Include/PlatformInclude.h"
#include "DataStructures/STRING_def.h"



void File_readFile_byte(char* p_absoluteFilePath, STRING_PTR out_file_byte);
void File_readFile_string(char* p_absoluteFilePath, STRING_PTR out_file_string);


struct FileStream;

#ifdef _WINDOWS

typedef struct FileStream
{
	FILE* Stream;
} FileStream;

#endif

typedef struct FileLineIterator
{
	FileStream FileStream;
	STRING Line;
	bool EndOfFile;
} FileLineIterator;

FileStream FileStream_open(const char* p_absoluteFilePath);
void FileStream_close(FileStream* p_fs);
FileLineIterator FileStream_allocLineIterator(FileStream* p_fs);
bool FileLineIterator_moveNext(FileLineIterator* p_fileLineIterator);
void FileLineIterator_free(FileLineIterator* p_fileLineIterator);

