#ifndef ENGINE_GLOBAL_CORE_THREAD_STATS_PROVIDER_HPP
#define ENGINE_GLOBAL_CORE_THREAD_STATS_PROVIDER_HPP
#pragma once

#include <chrono>
#include <thread>

namespace engine
{

    class thread_stats_t
    {

        public:

            virtual ~thread_stats_t()
            {

            }

            virtual std::chrono::duration<double> get_total_busy_time() = 0;
            virtual double get_current_cpu_usage() = 0;
        
    };

    class thread_stats_provider_base_t
    {

        public:

            virtual ~thread_stats_provider_base_t() {}

            virtual std::unique_ptr<thread_stats_t> get_stats_for_thread(std::thread::id id, std::thread::native_handle_type native_handle) = 0;

    };

	REGISTER_PROVIDER_BASE_TYPE(thread_stats_t, thread_stats_provider_base_t)

}

#include "global/core/thread/stats_provider/generic.hpp"
#include "global/core/thread/stats_provider/windows.hpp"

#endif