#include "Log.h"

#include "Constants.h"
#include <string.h>
#include <stdio.h>

#include "DataStructures/String.h"


inline void Arr_Alloc_LogMessage(Array_LogMessage_PTR p_array, size_t p_initlaCapacity) { Arr_Alloc(&p_array->array, sizeof(LogMessage), p_initlaCapacity); };
inline void Arr_Free_LogMessage(Array_LogMessage_PTR p_array) { Arr_Free(&p_array->array); };
inline void Arr_PushBackRealloc_LogMessage(Array_LogMessage_PTR p_array, LogMessage_PTR p_message) { Arr_PushBackRealloc(&p_array->array, sizeof(LogMessage), (char*)p_message); };

void MyLog_build(Log* p_myLog, Clock_PTR p_clock)
{
	p_myLog->Clock = p_clock;
	Arr_Alloc_LogMessage(&p_myLog->LogMessages, 0);
};

void MyLog_free(Log* p_myLog)
{
	MyLog_processLogs(p_myLog);
	Arr_Free_LogMessage(&p_myLog->LogMessages);
};

void MyLog_pushLog(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message)
{
	LogMessage l_logMessage = {0};
	l_logMessage.LogLevel = p_logLevel;
	l_logMessage.FilePath = p_filePath;
	l_logMessage.FileLine = p_line;

	{
		strcpy((char*)l_logMessage.Message, p_message);
	}

	{
		l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
	}

	Arr_PushBackRealloc_LogMessage(&p_myLog->LogMessages, &l_logMessage);
};

void MyLog_pushLog_string(Log* p_myLog, LogLevel p_logLevel, char* p_filePath, int p_line, String_PTR p_message)
{
	MyLog_pushLog(p_myLog, p_logLevel, p_filePath, p_line, (char*)p_message->Memory);
	String_Free(p_message);
};

void MyLog_processLogs(Log* p_myLog)
{
	for (size_t i = 0; i < p_myLog->LogMessages.Size; i++)
	{
		LogMessage_PTR l_message = &p_myLog->LogMessages.Memory[i];
		char* l_logLevemMessage = NULL;

		switch (l_message->LogLevel)
		{
		case LogLevel_INFO:
			l_logLevemMessage = " [INFO] - ";
			printf("\033[0;32m");
			break;

		case LogLevel_DEBUG:
			l_logLevemMessage = " [DEBUG] - ";
			printf("\033[1;32m");
			break;

		case LogLevel_WARN:
			l_logLevemMessage = " [WARN] - ";
			printf("\033[0;33m");
			break;

		case LogLevel_ERROR:
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

	Arr_Clear(&p_myLog->LogMessages.array);
};

