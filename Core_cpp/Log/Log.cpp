#include "Log.hpp"

#include "Constants.hpp"
#include <string.h>
#include <stdio.h>

namespace _Core
{
	void MyLog_build(Log* p_myLog, Clock* p_clock)
	{
		p_myLog->Clock = p_clock;
		VectorT_alloc(&p_myLog->LogMessages, 0);
	};

	void MyLog_free(Log* p_myLog)
	{
		MyLog_processLogs(p_myLog);
		VectorT_free(&p_myLog->LogMessages);
	};

	void MyLog_pushLog(Log* p_myLog,LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message)
	{
		LogMessage l_logMessage{};
		l_logMessage.LogLevel = p_logLevel;
		l_logMessage.FilePath = p_filePath;
		l_logMessage.FileLine = p_line;

		{
			strcpy((char*)l_logMessage.Message, p_message);
		}

		{
			l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
		}

		VectorT_pushBack(&p_myLog->LogMessages, &l_logMessage);
	};

	void MyLog_pushLog_string(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, String* p_message)
	{
		MyLog_pushLog(p_myLog, p_logLevel, p_filePath, p_line, (char*)p_message->Memory);
		String_free(p_message);
	};

	void MyLog_processLogs(Log* p_myLog)
	{
		VectorIteratorT<LogMessage> l_logMessageIt;
		VectorT_buildIterator(&p_myLog->LogMessages, &l_logMessageIt);
		while (VectorIteratorT_moveNext(&l_logMessageIt))
		{
			LogMessage* l_message = l_logMessageIt.Current;
			char* l_logLevemMessage = NULL;

			switch (l_message->LogLevel)
			{
			case LogLevel::LOG_INFO:
				l_logLevemMessage = " [INFO] - ";
				printf("\033[0;32m");
				break;

			case LogLevel::LOG_DEBUG:
				l_logLevemMessage = " [DEBUG] - ";
				printf("\033[1;32m");
				break;

			case LogLevel::LOG_WARN:
				l_logLevemMessage = " [WARN] - ";
				printf("\033[0;33m");
				break;

			case LogLevel::LOG_ERROR:
				l_logLevemMessage = " [ERROR] - ";
				printf("\033[0;31m");
				break;
			}

			char  l_frameCountStr[256] = "";
			ultoa(l_message->FrameNb, l_frameCountStr, 10);

			char l_lineCountStr[256] = "";
			itoa(l_message->FileLine, l_lineCountStr, 10);

			strcpy_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, l_frameCountStr);
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, l_logLevemMessage);
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, l_message->FilePath);
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, ".");
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, l_lineCountStr);
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, " ");
			strcat_s(p_myLog->TmpFinalMessage, sizeof p_myLog->TmpFinalMessage, l_message->Message);

			printf(p_myLog->TmpFinalMessage);
			printf("\n");
			printf("\033[0m");
		}
		VectorT_clear(&p_myLog->LogMessages);
	};

}

