#pragma once

#include <string.h>
#include <stdlib.h>

#include "DataStructures/GenericArray.h"
#include "Log/LogFormatting.h"
#include "Clock/Clock.h"

enum Core_LogLevel
{
	LOGLEVEL_INFO, LOGLEVEL_DEBUG, LOGLEVEL_WARN, LOGLEVEL_ERROR
};

#define LOG_MESSAGE_MAX_SIZE 1500

typedef struct Core_LogMessage
{
	enum Core_LogLevel LogLevel;
	char* FilePath;
	int FileLine;
	char Message[LOG_MESSAGE_MAX_SIZE];
	size_t FrameNb;
} Core_LogMessage;

#define LOG_FINAL_MESSAGE_MAX_SIZE 2000 

typedef struct Core_Log
{
	Core_GenericArray LogMessages;
	struct Core_Clock* Clock;

	char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
} Core_Log;

void MyLog_build(struct Core_Log* p_myLog, struct Core_Clock* p_clock);
void MyLog_free(struct Core_Log* p_myLog);
void MyLog_pushLog(struct Core_Log* p_myLog, enum Core_LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
void MyLog_pushLog_string(struct Core_Log* p_myLog, enum Core_LogLevel p_logLevel, char* p_filePath, int p_line, Core_GenericArray* p_message);
void MyLog_processLogs(struct Core_Log* p_myLog);

#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_STRING(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog_string(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)