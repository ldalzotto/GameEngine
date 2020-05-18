
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace _GameEngine
{
    Log *Log_alloc()
    {
        Log *l_log = new Log();
        spdlog::set_pattern("%^[%T] %n: %v%$");

        l_log->CoreLogger = spdlog::stdout_color_mt("GameEngine");
        l_log->CoreLogger->set_level(spdlog::level::trace);

        l_log->ClientLogger = spdlog::stdout_color_mt("App");
        l_log->ClientLogger->set_level(spdlog::level::trace);

        return l_log;
    };

    void Log_free(Log *p_log)
    {
        delete p_log;
    };

} // namespace _GameEngine
