#pragma once

extern "C"
{
#include "Include/CoreV2.h"
}
#include <string>

namespace _GameEngine::_Clock
{
	struct Clock;
}

namespace _GameEngine::_Log
{
	enum LogLevel : unsigned short
	{
		INFO, DEBUG, WARN, ERROR
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

	struct MyLog
	{
		Core_GenericArray LogMessages;
		_Clock::Clock* Clock;

		char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
	};

	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock);
	void MyLog_free(MyLog* p_myLog);
	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
	void MyLog_pushLog_string(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, Core_GenericArray* p_message);
	void MyLog_processLogs(MyLog* p_myLog);

	Core_String MyLog_formatError(char* p_file, int p_line, char* p_message);
	Core_GenericArray MyLog_formatError_string(const char* p_file, int p_line, Core_GenericArray* p_message);

#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) (char*)_Log::MyLog_formatError(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_BUILD_ERRORMESSAGE_STRING(MESSAGE) (char*)_Log::MyLog_formatError_string(__FILE__, __LINE__, MESSAGE).Memory
#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  _Log::MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_STRING(MYLOGPTR, LOGLEVEL, MESSAGE)  _Log::MyLog_pushLog_string(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
}