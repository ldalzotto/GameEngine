#pragma once

#include "DataStructures/GenericArray.h"

Core_GenericArray Core_log_formatError(char* p_file, int p_line, char* p_message);
Core_GenericArray Core_log_formatError_string(const char* p_file, int p_line, Core_GenericArray* p_message);

#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) (char*)Core_log_formatError(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_BUILD_ERRORMESSAGE_STRING(MESSAGE) (char*)Core_log_formatError_string(__FILE__, __LINE__, MESSAGE).Memory