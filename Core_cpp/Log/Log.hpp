#pragma once

#include <string.h>
#include <stdlib.h>

#include "DataStructures/Specifications/VectorT.hpp"
#include "DataStructures/Specifications/String.hpp"
#include "Log/LogFormatting.hpp"
#include "Clock/Clock.hpp"

namespace _Core
{
	struct Clock;
}

namespace _Core
{

	enum LogLevel : short int
	{
		LOGLEVEL_INFO, LOGLEVEL_DEBUG, LOGLEVEL_WARN, LOGLEVEL_ERROR
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
		Clock* Clock;

		char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
	};

	void MyLog_build(Log* p_myLog, Clock* p_clock);
	void MyLog_free(Log* p_myLog);
	void MyLog_pushLog(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
	void MyLog_pushLog_string(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, String* p_message);
	void MyLog_processLogs(Log* p_myLog);

#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_STRING(MYLOGPTR, LOGLEVEL, MESSAGE)  MyLog_pushLog_string(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
}
