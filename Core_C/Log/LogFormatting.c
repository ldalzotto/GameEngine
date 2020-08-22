#include "LogFormatting.h"

#include "DataStructures/String.h"
#include "Functional/ToString/ToString.h"


String Log_formatError(char* p_file, int p_line, char* p_message)
{
	String l_str;
	String_Alloc(&l_str, 100);

	char l_intContainer[50];
	ToString_intv(l_intContainer, &p_line);

	String_AppendRawRealloc(&l_str, p_file);
	String_AppendRawRealloc(&l_str, " ");
	String_AppendRawRealloc(&l_str, l_intContainer);
	String_AppendRawRealloc(&l_str, p_message);

	return l_str;
};

String Log_formatError_string(const char* p_file, int p_line, String_PTR p_message)
{
	String l_errorMessage;
	String_Alloc(&l_errorMessage, 0);

	String l_lineString;
	String_Alloc(&l_lineString, 0);

	ToString_int(&l_lineString, &p_line);

	String_AppendRawRealloc(&l_errorMessage, l_lineString.Memory);
	String_AppendRawRealloc(&l_errorMessage, p_message->Memory);

	String_Free(&l_lineString);
	String_Free(p_message);
	return l_errorMessage;
};
