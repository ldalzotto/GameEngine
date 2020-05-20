
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

namespace _GameEngine
{
	namespace _Log
	{
		Log *LogInstance = nullptr;

		void Log_alloc()
		{
			_Log::LogInstance = new Log();
			spdlog::set_pattern("%^[%T] %n: %v%$");

			LogInstance->CoreLogger = spdlog::stdout_color_mt("GameEngine");
			LogInstance->CoreLogger->set_level(spdlog::level::trace);

			LogInstance->ClientLogger = spdlog::stdout_color_mt("App");
			LogInstance->ClientLogger->set_level(spdlog::level::trace);
		};

		void Log_free(Log* p_log)
		{
			// delete p_log->ClientLogger.get();
			// delete p_log->CoreLogger.get();
			delete p_log;
		};

		std::string Log_formatError(const std::string& p_file, int p_line, const std::string& p_message)
		{
			return p_file + " " + std::to_string(p_line) + " : " + p_message;
		};
	} // namespace _Log

} // namespace _GameEngine
