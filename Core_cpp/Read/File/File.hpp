#pragma once

#include <stdio.h>
#include "Include/PlatformInclude.hpp"
#include "DataStructures/Specifications/String.hpp"


namespace _Core
{
	void File_readFile_byte(char* p_absoluteFilePath, String* out_file_byte);
	void File_readFile_string(char* p_absoluteFilePath, String* out_file_string);


	struct FileStream;

#ifdef _WINDOWS

	struct FileStream
	{
		FILE* Stream;
	};

#endif

	struct FileLineIterator
	{
		FileStream FileStream;
		String Line;
		bool EndOfFile;
	};

	FileStream FileStream_open(const char* p_absoluteFilePath);
	void FileStream_close(FileStream* p_fs);
	FileLineIterator FileStream_allocLineIterator(FileStream* p_fs);
	bool FileLineIterator_moveNext(FileLineIterator* p_fileLineIterator);
	void FileLineIterator_free(FileLineIterator* p_fileLineIterator);

}
