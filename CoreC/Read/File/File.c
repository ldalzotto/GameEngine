#include "File.h"

#include "ErrorHandling/Errorhandling.h"
#include "DataStructures/GenericArray.h"
#include "Log/LogFormatting.h"
#include "DataStructures/Specifications/String.h"
#include "Algorithm/String/StringAlgorithm.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorWriter.h"

#ifdef _WINDOWS

#include <Windows.h>
#include <stdio.h>

Core_ReturnCodes Core_File_readFile_byte(struct Core_GenericArray* p_absoluteFilePath, struct Core_GenericArray* out_file_byte)
{
	HANDLE l_file = CreateFile((char*)p_absoluteFilePath->Memory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (l_file == INVALID_HANDLE_VALUE)
	{
		Core_GenericArray l_errorMessage;
		Core_string_alloc(&l_errorMessage, 100);
		Core_string_append(&l_errorMessage, "Failed to open file : ");
		Core_string_append(&l_errorMessage, (char*)p_absoluteFilePath->Memory);
		ERR_PUSH_STRING(CR_OUT_OF_BOUND, &l_errorMessage);
	}

	LARGE_INTEGER l_size;
	if (!GetFileSizeEx(l_file, &l_size))
	{
		Core_GenericArray l_errorMessage;
		Core_string_alloc(&l_errorMessage, 100);
		Core_string_append(&l_errorMessage, "Failed to get file size : ");
		Core_string_append(&l_errorMessage, (char*)p_absoluteFilePath->Memory);
		ERR_PUSH_STRING(CR_OUT_OF_BOUND, &l_errorMessage);
	};

	Core_GenericArray_alloc(out_file_byte, sizeof(char), l_size.QuadPart);
	ReadFile(l_file, out_file_byte->Memory, l_size.QuadPart, 0, NULL);
	out_file_byte->Size = l_size.QuadPart;
	CloseHandle(l_file);

	return CR_OK;
};

Core_ReturnCodes Core_File_readFile_string(Core_GenericArray* p_absoluteFilePath, Core_GenericArray* out_file_string)
{
	ERR_PUSH(Core_File_readFile_byte(p_absoluteFilePath, out_file_string), "Core_File_readFile_string : error while trying to read the file.");
	char l_nullChar = NULL;
	Core_GenericArray_pushBack_realloc(out_file_string, &l_nullChar);
	return CR_OK;
};


#endif

