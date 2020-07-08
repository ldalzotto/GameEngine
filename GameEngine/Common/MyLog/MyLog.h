#pragma once

#include "../../CoreV3/Includes/CoreV3Include.hpp"
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
		_CoreV3::Char* FilePath;
		int FileLine;
		_CoreV3::Char Message[LOG_MESSAGE_MAX_SIZE];
		size_t FrameNb;
	};

#define LOG_FINAL_MESSAGE_MAX_SIZE 2000 

	struct MyLog
	{
		_CoreV3::VectorT<LogMessage> LogMessages;
		_Clock::Clock* Clock;

		char TmpFinalMessage[LOG_FINAL_MESSAGE_MAX_SIZE];
	};

	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock);
	void MyLog_free(MyLog* p_myLog);
	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, _CoreV3::Char* p_filePath, int p_line, _CoreV3::Char* p_message);
	void MyLog_pushLog_string(MyLog* p_myLog, LogLevel p_logLevel, _CoreV3::Char* p_filePath, int p_line, _CoreV3::String* p_message);
	void MyLog_processLogs(MyLog* p_myLog);

	std::string MyLog_formatError(const std::string& p_file, int p_line, const std::string& p_message);
	_CoreV3::String MyLog_formatError_string(const char* p_file, int p_line, _CoreV3::String&& p_message);

#define MYLOG_BUILD_ERRORMESSAGE(MESSAGE) _Log::MyLog_formatError(__FILE__, __LINE__, MESSAGE)
#define MYLOG_BUILD_ERRORMESSAGE_STRING(...) _Log::MyLog_formatError_string(__FILE__, __LINE__, _CoreV3::PushBackArrays(_CoreV3::Alloc<char>(20), __VA_ARGS__)).Memory)
#define MYLOG_PUSH(MYLOGPTR, LOGLEVEL, MESSAGE)  _Log::MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
#define MYLOG_PUSH_STRING(MYLOGPTR, LOGLEVEL, MESSAGE)  _Log::MyLog_pushLog(MYLOGPTR, LOGLEVEL,__FILE__, __LINE__, MESSAGE)
}