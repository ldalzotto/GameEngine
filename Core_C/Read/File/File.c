#include "File.h"

#include <stdio.h>
#include "DataStructures/STRING.h"



#ifdef _WINDOWS

#include <stdio.h>

	void File_readFile_byte(char* p_absoluteFilePath, STRING_PTR out_file_byte)
	{
		HANDLE l_file = CreateFile(p_absoluteFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (l_file == INVALID_HANDLE_VALUE)
		{
			STRING l_errorMessage;
			String_Alloc(&l_errorMessage, 100);
			String_AppendRaw(&l_errorMessage, "Failed to open file : ");
			String_AppendRaw(&l_errorMessage, p_absoluteFilePath);
			printf(l_errorMessage.Memory);
			abort();
		}

		LARGE_INTEGER l_size;
		if (!GetFileSizeEx(l_file, &l_size))
		{
			STRING l_errorMessage;
			String_Alloc(&l_errorMessage, 100);
			String_AppendRaw(&l_errorMessage, "Failed to get file size : ");
			String_AppendRaw(&l_errorMessage, p_absoluteFilePath);
			printf(l_errorMessage.Memory);
			abort();
		};

		String_Alloc(out_file_byte, l_size.QuadPart);
		ReadFile(l_file, out_file_byte->Memory, l_size.QuadPart, 0, NULL);
		out_file_byte->Size = l_size.QuadPart;
		CloseHandle(l_file);
	};

	void File_readFile_string(char* p_absoluteFilePath, STRING_PTR out_file_string)
	{
		File_readFile_byte(p_absoluteFilePath, out_file_string);
		char l_nullChar = NULL;
		String_AppendRaw(out_file_string, &l_nullChar);
	};


	FileStream FileStream_open(const char* p_absoluteFilePath)
	{
		FileStream l_fs;
		l_fs.Stream = fopen(p_absoluteFilePath, "r");
		return l_fs;
	};

	void FileStream_close(FileStream* p_fs)
	{
		fclose(p_fs->Stream);
	};

	FileLineIterator FileStream_allocLineIterator(FileStream* p_fs)
	{
		FileLineIterator l_it = {0};
		l_it.FileStream = *p_fs;
		String_Alloc(&l_it.Line, 0);
		return l_it;
	};

	bool FileLineIterator_moveNext(FileLineIterator* p_fileLineIterator)
	{
		if (p_fileLineIterator->EndOfFile)
		{
			return false;
		}

		String_Clear(&p_fileLineIterator->Line);
		int c = fgetc(p_fileLineIterator->FileStream.Stream);
		while (true)
		{
			// line return
			if (c == '\n')
			{
				return true;
			}
			else if (c == EOF)
			{
				p_fileLineIterator->EndOfFile = true;
				return true;
			}
			else
			{
				String_AppendRaw(&p_fileLineIterator->Line, (char*)&c);
			}

			c = fgetc(p_fileLineIterator->FileStream.Stream);
		}
	};

	void FileLineIterator_free(FileLineIterator* p_fileLineIterator)
	{
		String_Free(&p_fileLineIterator->Line);
	};

#endif
