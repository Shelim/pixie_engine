#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/thread_accounter/real.hpp"
#include <windows.h>
namespace engine
{

    class thread_accounter_provider_windows_rescan_service_t : public service_base_t
    {

    public:

        ustring_t get_name() const final
        {
            return "Thread accounter (Windows provider) service"_u;
        }

        task_base_t::result_t run() final
        {
            if(flags.is_flag(flag_t::requested_shutdown))
                return task_base_t::result_t::completed;
            return task_base_t::result_t::running;
        }

        void on_end_requested() final
        {
            flags.set_flag(flag_t::requested_shutdown, true);
        }
        
        std::chrono::duration<double> get_sleep_after() const final
        {
            return std::chrono::duration<double>(1.0);
        }

    private:

        enum class flag_t
        {
            requested_shutdown,
            count
        };

        flags_t<flag_t> flags;

    };


    class thread_accounter_provider_windows_t : public thread_accounter_provider_base_t
	{

    public:

        thread_accounter_provider_windows_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<thread_accounter_provider_windows_rescan_service_t>> service) : thread_accounter_provider_base_t(logger), service(std::move(service))
        {
            auto task_id = get_logger()->log_global_task_start(threads, "Initializing accounter [thread] Windows provider"_u);
			this->service->start();
			get_logger()->log_global_task_done(task_id);
        }

        ~thread_accounter_provider_windows_t()
        {
            auto task_id = get_logger()->log_global_task_start(threads, "Accounter [thread] Windows provider is being disposed"_u);
			this->service->end();
			get_logger()->log_global_task_done(task_id);
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

        std::unique_ptr<service_t<thread_accounter_provider_windows_rescan_service_t>> service;

		std::unique_ptr<thread_info_t> create_thread_info(thread_t * thread) final
        {
            return std::make_unique<thread_info_windows_t>(thread);
        }

    };

}

#endif

#endif