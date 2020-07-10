#include "Log.h"

#include "Constants.h"
#include "DataStructures/Specifications/Vector.h"
#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"

void MyLog_build(struct Core_Log* p_myLog, struct Core_Clock* p_clock)
{
	p_myLog->Clock = p_clock;
	Core_Vector_alloc(&p_myLog->LogMessages, sizeof(Core_LogMessage), 0);
};

void MyLog_free(struct Core_Log* p_myLog)
{
	MyLog_processLogs(p_myLog);
	Core_GenericArray_free(&p_myLog->LogMessages);
};

void MyLog_pushLog(struct Core_Log* p_myLog, enum Core_LogLevel p_logLevel, char* p_filePath, int p_line, char* p_message)
{
	Core_LogMessage l_logMessage;
	ZEROING(Core_LogMessage, &l_logMessage);

	l_logMessage.LogLevel = p_logLevel;
	l_logMessage.FilePath = p_filePath;
	l_logMessage.FileLine = p_line;

	{
		strcpy((char*)l_logMessage.Message, p_message);
	}

	{
		l_logMessage.FrameNb = p_myLog->Clock->FrameCount;
	}

	p_myLog->LogMessages.Functions->Writer->PushBack(&p_myLog->LogMessages, &l_logMessage);
};

void MyLog_pushLog_string(struct Core_Log* p_myLog, enum Core_LogLevel p_logLevel, char* p_filePath, int p_line, Core_GenericArray* p_message)
{
	MyLog_pushLog(p_myLog, p_logLevel, p_filePath, p_line, (char*)p_message->Memory);
	Core_GenericArray_free(p_message);
};

void MyLog_processLogs(struct Core_Log* p_myLog)
{
	for (size_t i = 0; i < p_myLog->LogMessages.Size; i++)
	{
		Core_LogMessage* l_message = p_myLog->LogMessages.Functions->Accessor->At(&p_myLog->LogMessages, i);
		char* l_logLevemMessage = NULL;

		switch (l_message->LogLevel)
		{
		case LOGLEVEL_INFO:
			l_logLevemMessage = " [INFO] - ";
			printf("\033[0;32m");
			break;

		case LOGLEVEL_DEBUG:
			l_logLevemMessage = " [DEBUG] - ";
			printf("\033[1;32m");
			break;

		case LOGLEVEL_WARN:
			l_logLevemMessage = " [WARN] - ";
			printf("\033[0;33m");
			break;

		case LOGLEVEL_ERROR:
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

		// logMessage_free(l_message);
	}

	p_myLog->LogMessages.Functions->Writer->Clear(&p_myLog->LogMessages);

};