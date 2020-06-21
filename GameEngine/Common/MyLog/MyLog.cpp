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
		p_myLog->GarbageAllocations.alloc();
	};

	void MyLog_free(MyLog* p_myLog)
	{
		MyLog_processLogs(p_myLog);
		p_myLog->LogMessages.free();
		p_myLog->GarbageAllocations.free();
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

			char l_lineCountStr[256] = "";
			itoa(l_message->FileLine, l_lineCountStr, 10);

			strcpy(p_myLog->TmpFinalMessage, l_frameCountStr);
			strcat(p_myLog->TmpFinalMessage, l_logLevemMessage);
			strcat(p_myLog->TmpFinalMessage, l_message->FilePath);
			strcat(p_myLog->TmpFinalMessage, ".");
			strcat(p_myLog->TmpFinalMessage, l_lineCountStr);
			strcat(p_myLog->TmpFinalMessage, " ");
			strcat(p_myLog->TmpFinalMessage, l_message->Message);

			printf(p_myLog->TmpFinalMessage);
			printf("\n");
			printf("\033[0m");

			logMessage_free(l_message);
		}

		p_myLog->LogMessages.clear();

		for (size_t i = 0; i < p_myLog->GarbageAllocations.size(); i++)
		{
			void* l_garbageMemory = *p_myLog->GarbageAllocations.at(i);
			free(l_garbageMemory);
		}
		p_myLog->GarbageAllocations.clear();
	};

	char* MyLog_allocateString(MyLog* p_myLog, size_t p_size)
	{
		void* l_allocatedMemory = malloc(p_size);
		p_myLog->GarbageAllocations.push_back(&l_allocatedMemory);
		return (char*)l_allocatedMemory;
	};

	char* MyLog_allocatePointerDouble(MyLog* p_myLog, double p_double)
	{
		void* l_allocatedMemory = malloc(sizeof(char) * 50);
		p_myLog->GarbageAllocations.push_back(&l_allocatedMemory);
		snprintf((char*)l_allocatedMemory, sizeof(char) * 50, "%f", p_double);
		return (char*)l_allocatedMemory;
	};

	char* MyLog_allocatePointerString(MyLog* p_myLog, void* p_ptr)
	{
		void* l_allocatedMemory = malloc(sizeof(char) * 19);
		p_myLog->GarbageAllocations.push_back(&l_allocatedMemory);
		snprintf((char*)l_allocatedMemory, sizeof(char) * 19, "0x%p", p_ptr);
		return (char*)l_allocatedMemory;
	};

	std::string MyLog_formatError(const std::string& p_file, int p_line, const std::string& p_message)
	{
		return p_file + " " + std::to_string(p_line) + " : " + p_message;
	};
}