#pragma once

#include "DataStructures/String_def.h"

String Log_formatError(char* p_file, int p_line, char* p_message);
String Log_formatError_string(const char* p_file, int p_line, String_PTR p_message);

#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) Log_formatError(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_BUILD_ERRORMESSAGE_String(MESSAGE) Log_formatError_string(__FILE__, __LINE__, MESSAGE).Memory