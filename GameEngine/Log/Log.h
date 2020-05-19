#pragma once

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
        extern Log *LogInstance;

        void Log_alloc();
        void Log_free(Log *p_log);
    } // namespace _Log

} // namespace _GameEngine