#pragma once

extern "C"
{
#include "DataStructures/STRING_def.h"
}

namespace _Core
{
	STRING Log_formatError(char* p_file, int p_line, char* p_message);
	STRING Log_formatError_string(const char* p_file, int p_line, STRING_PTR p_message);
}


#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) ::_Core::Log_formatError(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_BUILD_ERRORMESSAGE_STRING(MESSAGE) ::_Core::Log_formatError_string(__FILE__, __LINE__, MESSAGE).Memory