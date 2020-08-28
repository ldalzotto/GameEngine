#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "Include/PlatformInclude.h"
#include "DataStructures/String_def.h"



void File_readFile_byte(char* p_absoluteFilePath, String_PTR out_file_byte);
void File_readFile_string(char* p_absoluteFilePath, String_PTR out_file_string);


struct FileStream;

#ifdef _WINDOWS

typedef struct FileStream
{
	FILE* Stream;
} FileStream;

#endif

typedef char FILESTREAM_MODE;
#define FILESTREAM_MODE_READ 0
#define FILESTREAM_MODE_WRITE 1

typedef struct FileLineIterator
{
	FileStream FileStream;
	String Line;
	bool EndOfFile;
} FileLineIterator;

FileStream FileStream_open(const char* p_absoluteFilePath, const FILESTREAM_MODE p_mode);
void FileStream_close(FileStream* p_fs);
FileLineIterator FileStream_allocLineIterator(FileStream* p_fs);
void FileStream_writeSyncRaw(FileStream* p_fs, const char* p_buffer, size_t p_bufferSize);
bool FileLineIterator_moveNext(FileLineIterator* p_fileLineIterator);
void FileLineIterator_free(FileLineIterator* p_fileLineIterator);

