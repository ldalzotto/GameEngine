#include "File.hpp"

#include <stdexcept>
#include "DataStructures/Specifications/String.hpp"

namespace _Core
{

#ifdef _WINDOWS

#include "Include/PlatformInclude.hpp"

#include <stdio.h>

	void File_readFile_byte(char* p_absoluteFilePath, String* out_file_byte)
	{
		HANDLE l_file = CreateFile(p_absoluteFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (l_file == INVALID_HANDLE_VALUE)
		{
			String l_errorMessage;
			String_alloc(&l_errorMessage, 100);
			String_append(&l_errorMessage, "Failed to open file : ");
			String_append(&l_errorMessage, p_absoluteFilePath);
			throw std::runtime_error(l_errorMessage.Memory);
		}

		LARGE_INTEGER l_size;
		if (!GetFileSizeEx(l_file, &l_size))
		{
			String l_errorMessage;
			String_alloc(&l_errorMessage, 100);
			String_append(&l_errorMessage, "Failed to get file size : ");
			String_append(&l_errorMessage, p_absoluteFilePath);
			throw std::runtime_error(l_errorMessage.Memory);
		};

		String_alloc(out_file_byte, l_size.QuadPart);
		ReadFile(l_file, out_file_byte->Memory, l_size.QuadPart, 0, NULL);
		out_file_byte->Size = l_size.QuadPart;
		CloseHandle(l_file);
	};

	void Core_File_readFile_string(char* p_absoluteFilePath, String* out_file_string)
	{
		File_readFile_byte(p_absoluteFilePath, out_file_string);
		char l_nullChar = NULL;
		String_append(out_file_string, &l_nullChar);
	};


#endif



}


