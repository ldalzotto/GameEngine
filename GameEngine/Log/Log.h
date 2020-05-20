#pragma once

#include <string>

#include "spdlog/spdlog.h"

namespace _GameEngine
{

	struct Log
	{
		std::shared_ptr<spdlog::logger> CoreLogger;
		std::shared_ptr<spdlog::logger> ClientLogger;
	};

	namespace _Log
	{
		extern Log* LogInstance;

		void Log_alloc();
		void Log_free(Log** p_log);

		std::string Log_formatError(const std::string& p_file, int p_line, const std::string& p_message);
	} // namespace _Log

#define LOG_BUILD_ERRORMESSAGE(MESSAGE) _Log::Log_formatError(__FILE__, __LINE__, MESSAGE)

} // namespace _GameEngine