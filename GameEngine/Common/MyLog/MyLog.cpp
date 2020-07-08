#include "MyLog.h"

#include "Clock/Clock.h"

namespace _GameEngine::_Log
{
	void MyLog_build(MyLog* p_myLog, _Clock::Clock* p_clock)
	{
		p_myLog->Clock = p_clock;
		_CoreV3::Alloc(&p_myLog->LogMessages, 0);
	};

	void MyLog_free(MyLog* p_myLog)
	{
		MyLog_processLogs(p_myLog);
		_CoreV3::Free(&p_myLog->LogMessages);
	};

	void logMessage_free(LogMessage* p_logMessage)
	{
		//	free(p_logMessage->Message);
	};

	void MyLog_pushLog(MyLog* p_myLog, LogLevel p_logLevel, _CoreV3::Char* p_filePath, int p_line, _CoreV3::Char* p_message)
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

		_CoreV3::PushBack(&p_myLog->LogMessages, &l_logMessage);
	};

	void MyLog_pushLog_string(MyLog* p_myLog, LogLevel p_logLevel, _CoreV3::Char* p_filePath, int p_line, _CoreV3::String* p_message)
	{
		MyLog_pushLog(p_myLog, p_logLevel, p_filePath, p_line, p_message->Memory);
		Free(p_message);
	};

	void MyLog_processLogs(MyLog* p_myLog)
	{
		for (size_t i = 0; i < p_myLog->LogMessages.Size; i++)
		{
			LogMessage* l_message = At(&p_myLog->LogMessages, i);
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

		_CoreV3::Clear(&p_myLog->LogMessages);

	};

	std::string MyLog_formatError(const std::string& p_file, int p_line, const std::string& p_message)
	{
		return p_file + " " + std::to_string(p_line) + " : " + p_message;
	};

	_CoreV3::String MyLog_formatError_string(const char* p_file, int p_line, _CoreV3::String&& p_message)
	{
		_CoreV3::String l_errorMessage = _CoreV3::Alloc<char>(0);
		_CoreV3::String l_lineString = _CoreV3::ToString(&p_line);
		
		_CoreV3::PushBackArrays(&l_errorMessage, STR(p_file), &l_lineString, &p_message);

		Free(&l_lineString);
		Free(&p_message);
		return std::move(l_errorMessage);
	};
}