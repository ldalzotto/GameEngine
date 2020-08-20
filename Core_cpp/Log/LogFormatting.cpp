#include "LogFormatting.hpp"
#include "Functional/ToString/ToString.hpp"
extern "C"
{
#include "DataStructures/STRING.h"
}


namespace _Core
{
	STRING Log_formatError(char* p_file, int p_line, char* p_message)
	{
		STRING l_str;
		String_Alloc(&l_str, 100);

		char l_intContainer[50];
		ToString_intv(l_intContainer, &p_line);

		String_AppendRaw(&l_str, p_file);
		String_AppendRaw(&l_str, " ");
		String_AppendRaw(&l_str, l_intContainer);
		String_AppendRaw(&l_str, p_message);

		return l_str;
	};

	STRING Log_formatError_string(const char* p_file, int p_line, STRING_PTR p_message)
	{
		STRING l_errorMessage;
		String_Alloc(&l_errorMessage, 0);

		STRING l_lineString;
		String_Alloc(&l_lineString, 0);

		ToString_int(&l_lineString, &p_line);

		String_AppendRaw(&l_errorMessage, (char*)l_lineString.Memory);
		String_AppendRaw(&l_errorMessage, (char*)p_message->Memory);

		String_Free(&l_lineString);
		String_Free(p_message);
		return l_errorMessage;
	};

}
