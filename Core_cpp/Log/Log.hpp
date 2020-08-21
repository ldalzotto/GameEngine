#pragma once

#include <string.h>
#include <stdlib.h>

#include "DataStructures/Specifications/VectorT.hpp"
extern "C"
{
#include "DataStructures/String_def.h"
#include "Clock/Clock.h"
}
#include "Log/LogFormatting.hpp"

namespace _Core
{

	enum LogLevel : short int
	{
		LOG_INFO, 
		LOG_DEBUG, 
		LOG_WARN, 
		LOG_ERROR
	};

#define LOG_MESSAGE_MAX_SIZE 1500

	struct LogMessage
	{
		LogLevel LogLevel;
		char* FilePath;
		int FileLine;
		char Message[LOG_MESSAGE_MAX_SIZE];
		size_t FrameNb;
	};

#define LOG_FINAL_MESSAGE_MAX_SIZE 2000 

	struct Log
	{
		VectorT<LogMessage> LogMessages;
		Clock_PTR Clock;

		char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
	};

	void MyLog_build(Log* p_myLog, Clock_PTR p_clock);
	void MyLog_free(Log* p_myLog);
	void MyLog_pushLog(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
	void MyLog_pushLog_string(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, String_PTR p_message);
	void MyLog_processLogs(Log* p_myLog);

#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_String(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog_string(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
}
