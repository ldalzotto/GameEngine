#pragma once

#include <string.h>
#include <stdlib.h>

#include "Log_def.h"
 
void MyLog_build(Log* p_myLog, Clock_PTR p_clock);
void MyLog_free(Log* p_myLog);
void MyLog_pushLog(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
void MyLog_pushLog_string(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, String_PTR p_message);
void MyLog_processLogs(Log* p_myLog);

#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_String(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog_string(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
