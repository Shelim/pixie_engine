#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/core/thread/stats_provider.hpp"

#include <windows.h>

#undef min
#undef max
namespace engine
{

    class thread_stats_provider_windows_t : public thread_stats_provider_base_t
	{

    public:

        std::unique_ptr<thread_stats_t> get_stats_for_thread(std::thread::id id, std::thread::native_handle_type native_handle) final
        {
            return std::make_unique<thread_stats_windows_t>(native_handle);
        }

    private:

        class thread_stats_windows_t : public thread_stats_t
        {

            public:

                thread_stats_windows_t(std::thread::native_handle_type native_handle) : native_handle(native_handle), last_time(0), last_cpu(0)
                {

                }

                std::chrono::duration<double> get_total_busy_time() final
                {
                    FILETIME creation_time, exit_time, kernel_busy, user_busy;

                    if(!GetThreadTimes(native_handle, &creation_time, &exit_time, &kernel_busy, &user_busy))
                    {
                        DWORD error = GetLastError();
                        return std::chrono::duration<double>::zero();
                    }

                    return std::chrono::duration<double>(static_cast<double>((convert(kernel_busy) + convert(user_busy)) / 10000000.0l));
                }

                double get_current_cpu_usage() final
                {
                    FILETIME creation_time, exit_time, kernel_busy, user_busy;

                    GetThreadTimes(native_handle, &creation_time, &exit_time, &kernel_busy, &user_busy);

                    uint64_t cpu = convert(kernel_busy) + convert(user_busy);
                    uint64_t time = convert(exit_time) - convert(creation_time);

                    if(last_time == 0 || time > last_time + 10000000ll)
                    {
                        last_time = time;
                        last_cpu = cpu;
                    }

                    return static_cast<double>(round((last_cpu / 10000000.0l) / (last_time / 10000000.0l)));
                }

            private:

                uint64_t convert(const FILETIME & filetime)
                {
                    ULARGE_INTEGER large;

                    large.LowPart  = filetime.dwLowDateTime;
                    large.HighPart = filetime.dwHighDateTime;

                    return large.QuadPart;
                }  

                uint64_t last_time;
                uint64_t last_cpu;

                std::thread::native_handle_type native_handle;

        };

    };

}

#endif

#endif