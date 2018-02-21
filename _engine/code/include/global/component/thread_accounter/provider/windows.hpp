#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/thread_accounter/real.hpp"
#include <windows.h>
namespace engine
{

    class thread_accounter_provider_windows_t : public thread_accounter_provider_base_t
	{

    public:

        thread_accounter_provider_windows_t(std::shared_ptr<logger_t> logger) : thread_accounter_provider_base_t(logger)
        {

        }

        ~thread_accounter_provider_windows_t()
        {
            
        }

    private:

        class thread_info_windows_t : public thread_info_t
        {

        public:

            thread_info_windows_t(thread_t * thread) : thread_info_t(thread), cpu_usage_in_last_second(0.0)
            {
                hthread = (HANDLE) thread->get_native_handle();
            }

            std::chrono::duration<double> get_cpu_usage_in_last_second() const final
            {
                return std::chrono::duration<double>(cpu_usage_in_last_second);
            }

        private:

            double cpu_usage_in_last_second;
            HANDLE hthread;

        };

		std::unique_ptr<thread_info_t> create_thread_info(thread_t * thread) final
        {
            return std::make_unique<thread_info_windows_t>(thread);
        }

    };

}

#endif

#endif