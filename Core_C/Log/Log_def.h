#pragma once

#include "Clock/Clock.h"
#include "DataStructures/ARRAY_def.h"
#include "DataStructures/String_def.h"

typedef short int LogLevel;
#define LogLevel_INFO 0
#define LogLevel_DEBUG 1
#define LogLevel_WARN 2
#define LogLevel_ERROR 3

#define LOG_MESSAGE_MAX_SIZE 1500

typedef struct LogMessage_TYP
{
	LogLevel LogLevel;
	char* FilePath;
	int FileLine;
	char Message[LOG_MESSAGE_MAX_SIZE];
	size_t FrameNb;
}LogMessage, * LogMessage_PTR;

typedef struct Array_LogMessage_TYP
{
	ARRAY_TYPE_DEFINITION(LogMessage)
}Array_LogMessage, * Array_LogMessage_PTR;

#define LOG_FINAL_MESSAGE_MAX_SIZE 2000 

typedef struct Log_TYP
{
	Array_LogMessage LogMessages;
	Clock_PTR Clock;

	char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
}Log, * Log_PTR;