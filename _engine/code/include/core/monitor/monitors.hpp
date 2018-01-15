#ifndef ENGINE_COMMON_UTILITY_MONITORS_HPP
#define ENGINE_COMMON_UTILITY_MONITORS_HPP

#pragma once

#include <memory>
#include "component/logger.hpp"
#include "component/config_monitor.hpp"

namespace engine
{

    class monitors_t
    {

    public:

        monitors_t(
#define ENGINE_MONITOR_DEF(name) std::shared_ptr<name##_monitor_t> name##_monitor,
#include "def/monitor.def"
            std::shared_ptr<logger_t> logger
        ) : 
#define ENGINE_MONITOR_DEF(name) name##_monitor(name##_monitor),
#include "def/monitor.def"
             logger(logger)
        {
            logger->log_msg(core, "Monitors are ready!"_u);
        }

        ~monitors_t()
        {
            logger->log_msg(core, "Monitors being disposed"_u);
        }

    private:

#define ENGINE_MONITOR_DEF(name) std::shared_ptr<name##_monitor_t> name##_monitor;
#include "def/monitor.def"
        std::shared_ptr<logger_t> logger;

    };

}

#endif