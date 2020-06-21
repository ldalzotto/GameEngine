#pragma once

#include "DataStructures/VectorT.h"
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
		_Core::VectorT<LogMessage> LogMessages;
		_Clock::Clock* Clock;
		// TODO -> Using a custom memory allocator tied to the log object.
		_Core::VectorT<void*> GarbageAllocations;
		char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
	};

	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock);
	void MyLog_free(MyLog* p_myLog);
	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message);
	void MyLog_processLogs(MyLog* p_myLog);

	char* MyLog_allocateString(MyLog* p_myLog, size_t p_size);
	char* MyLog_allocatePointerDouble(MyLog* p_myLog, double p_double);
	char* MyLog_allocatePointerString(MyLog* p_myLog, void* p_ptr);
	std::string MyLog_formatError(const std::string& p_file, int p_line, const std::string& p_message);

#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) _Log::MyLog_formatError(__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  _Log::MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
}