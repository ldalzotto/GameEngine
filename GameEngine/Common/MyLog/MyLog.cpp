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
	};

	void MyLog_free(MyLog* p_myLog)
	{
		p_myLog->LogMessages.free();
		free(p_myLog->TmpFinalMessage);
	};

	void logMessage_free(LogMessage* p_logMessage)
	{
		free(p_logMessage->Message);
	};

	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, char* p_message)
	{
		LogMessage l_logMessage{};

		l_logMessage.LogLevel = p_logLevel;

		{
			l_logMessage.Message = p_message;
			l_logMessage.Message = (char*)malloc(strlen(p_message) + 1);
			strcpy(l_logMessage.Message, p_message);
		}

		{
			l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
		}
		/*
		{
			time_t rawtime;
			struct tm* timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			char* l_ascTime = asctime(timeinfo);
			size_t len = strlen(l_ascTime);
			l_ascTime[len - 1] = '\0';
			l_logMessage.TimeString = (char*)malloc(strlen(l_ascTime) + 1);
			strcpy(l_logMessage.TimeString, l_ascTime);
		}
		*/

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
			
			char* l_newFinalMessage = (char*)realloc(p_myLog->TmpFinalMessage, strlen(l_frameCountStr) + strlen(l_message->Message) + strlen(l_logLevemMessage) + 1);
			if (l_newFinalMessage == nullptr)
			{
				free(p_myLog->TmpFinalMessage);
			}
			p_myLog->TmpFinalMessage = l_newFinalMessage;
			strcpy(p_myLog->TmpFinalMessage, l_frameCountStr);
			strcat(p_myLog->TmpFinalMessage, l_logLevemMessage);
			strcat(p_myLog->TmpFinalMessage, l_message->Message);

			printf(p_myLog->TmpFinalMessage);
			printf("\n");
			printf("\033[0m");

			logMessage_free(l_message);
		}

		p_myLog->LogMessages.clear();
	};
}