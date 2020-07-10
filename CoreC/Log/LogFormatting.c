#include "LogFormatting.h"

#include "Algorithm/String/StringAlgorithm.h"
#include "Functional/ToString/ToString.h"

Core_GenericArray Core_log_formatError(char* p_file, int p_line, char* p_message)
{
	Core_GenericArray l_str;
	Core_string_alloc(&l_str, 100);

	char l_intContainer[50];
	Core_toString_intv(l_intContainer, &p_line);

	Core_string_append(&l_str, p_file);
	Core_string_append(&l_str, " ");
	Core_string_append(&l_str, l_intContainer);
	Core_string_append(&l_str, p_message);

	return l_str;
};

Core_GenericArray Core_log_formatError_string(const char* p_file, int p_line, Core_GenericArray* p_message)
{
	Core_GenericArray l_errorMessage;
	Core_string_alloc(&l_errorMessage, 0);

	Core_GenericArray l_lineString;
	Core_string_alloc(&l_lineString, 0);

	Core_toString_int(&l_lineString, &p_line);

	Core_string_append(&l_errorMessage, (char*)l_lineString.Memory);
	Core_string_append(&l_errorMessage, (char*)p_message->Memory);

	Core_GenericArray_free(&l_lineString);
	Core_GenericArray_free(p_message);
	return l_errorMessage;
};