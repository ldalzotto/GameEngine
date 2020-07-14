#include "LogFormatting.hpp"
#include "Functional/ToString/ToString.hpp"

namespace _Core
{
	String Core_log_formatError(char* p_file, int p_line, char* p_message)
	{
		String l_str;
		String_alloc(&l_str, 100);

		char l_intContainer[50];
		ToString_intv(l_intContainer, &p_line);

		String_append(&l_str, p_file);
		String_append(&l_str, " ");
		String_append(&l_str, l_intContainer);
		String_append(&l_str, p_message);

		return l_str;
	};

	String Core_log_formatError_string(const char* p_file, int p_line, String* p_message)
	{
		String l_errorMessage;
		String_alloc(&l_errorMessage, 0);

		String l_lineString;
		String_alloc(&l_lineString, 0);

		ToString_int(&l_lineString, &p_line);

		String_append(&l_errorMessage, (char*)l_lineString.Memory);
		String_append(&l_errorMessage, (char*)p_message->Memory);

		String_free(&l_lineString);
		String_free(p_message);
		return l_errorMessage;
	};

}
