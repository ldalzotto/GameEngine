#pragma once

#include "DataStructures/VectorT.h"

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

	struct LogMessage
	{
		LogLevel LogLevel;
		char* Message;
		size_t FrameNb;
	};

	struct MyLog
	{
		_Core::VectorT<LogMessage> LogMessages;
		_Clock::Clock* Clock;
		char* TmpFinalMessage;
	};

	MyLog* MyLog_alloc(_Clock::Clock* p_clock);
	void MyLog_free(MyLog** p_myLog);
	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_message);
	void MyLog_processLogs(MyLog* p_myLog);
}