#pragma once

#include "DataStructures/Specifications/String.h"

Core_String Core_log_formatError(char* p_file, int p_line, char* p_message);
Core_GenericArray Core_log_formatError_string(const char* p_file, int p_line, struct Core_String* p_message);