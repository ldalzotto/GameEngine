#include "MyLog.h"
#include <stdio.h>
#include <time.h>  
#include <cstdlib>
#include <string.h>

#include "Clock/Clock.h"

namespace _GameEngine::_Log
{
	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock)
	{
		p_myLog->Clock = p_clock;
		p_myLog->LogMessages.alloc();
		p_myLog->StringAllocations.alloc();
	};

	void MyLog_free(MyLog* p_myLog)
	{
		MyLog_processLogs(p_myLog);
		p_myLog->LogMessages.free();
		p_myLog->StringAllocations.free();
	};

	void logMessage_free(LogMessage* p_logMessage)
	{
		//	free(p_logMessage->Message);
	};

	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message)
	{
		LogMessage l_logMessage{};
		l_logMessage.LogLevel = p_logLevel;
		l_logMessage.FilePath = p_filePath;
		l_logMessage.FileLine = p_line;

		{
			strcpy_s(l_logMessage.Message, p_message);
		}

		{
			l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
		}
		  
		p_myLog->LogMessages.push_back(&l_logMessage);
	};

	void MyLog_processLogs(MyLog* p_myLog)
	{
		for (size_t i = 0; i < p_myLog->LogMessages.size(); i++)
		{
			LogMessage* l_message = p_myLog->LogMessages.at(i);
			char* l_logLevemMessage = nullptr;

			switch (l_message->LogLevel)
			{
			case LogLevel::INFO:
				l_logLevemMessage = " [INFO] - ";
				printf("\033[0;32m");
				break;

			case LogLevel::DEBUG:
				l_logLevemMessage = " [DEBUG] - ";
				printf("\033[1;32m");
				break;

			case LogLevel::WARN:
				l_logLevemMessage = " [WARN] - ";
				printf("\033[0;33m");
				break;

			case LogLevel::ERROR:
				l_logLevemMessage = " [ERROR] - ";
				printf("\033[0;31m");
				break;
			}

			char  l_frameCountStr[256] = "";
			ultoa(l_message->FrameNb, l_frameCountStr, 10);

			char l_lineCountStr[256] = "";
			itoa(l_message->FileLine, l_lineCountStr, 10);

			strcpy_s(p_myLog->TmpFinalMessage, l_frameCountStr);
			strcat_s(p_myLog->TmpFinalMessage, l_logLevemMessage);
			strcat_s(p_myLog->TmpFinalMessage, l_message->FilePath);
			strcat_s(p_myLog->TmpFinalMessage, ".");
			strcat_s(p_myLog->TmpFinalMessage, l_lineCountStr);
			strcat_s(p_myLog->TmpFinalMessage, " ");
			strcat_s(p_myLog->TmpFinalMessage, l_message->Message);

			printf(p_myLog->TmpFinalMessage);
			printf("\n");
			printf("\033[0m");

			logMessage_free(l_message);
		}

		p_myLog->LogMessages.clear();

		for (size_t i = 0; i < p_myLog->StringAllocations.size(); i++)
		{
			_Core::String* l_garbageMemory = p_myLog->StringAllocations.at(i);
			l_garbageMemory->free();
		}

		p_myLog->StringAllocations.clear();
	};

	_Core::String* MyLog_AllocateString(MyLog* p_mylog)
	{
		_Core::String l_str;
		// l_str.alloc(LOG_MESSAGE_MAX_SIZE);
		p_mylog->StringAllocations.push_back(&l_str);
		_Core::String* l_insertedString = p_mylog->StringAllocations.at(p_mylog->StringAllocations.size() - 1);
		l_insertedString->alloc(LOG_MESSAGE_MAX_SIZE);
		return l_insertedString;
	};

	std::string MyLog_formatError(const std::string& p_file, int p_line, const std::string& p_message)
	{
		return p_file + " " + std::to_string(p_line) + " : " + p_message;
	};
}