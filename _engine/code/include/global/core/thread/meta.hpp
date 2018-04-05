#ifndef ENGINE_GLOBAL_CORE_THREAD_META_HPP
#define ENGINE_GLOBAL_CORE_THREAD_META_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "global/core/app.hpp"
#include "utility/pattern/provider.hpp"
#include "global/core/thread/stats_provider.hpp"
#include <chrono>
#include <functional>

namespace engine
{

    class thread_meta_t
    {

        public:

            thread_meta_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::thread::id id, std::thread::native_handle_type native_handle, std::unique_ptr<thread_stats_t> thread_stats) :
                app(app), app_instance_id(app_instance_id), name(name), id(id), native_handle(native_handle), creation_time(std::chrono::steady_clock::now()), thread_stats(std::move(thread_stats)), total_busy_time(std::chrono::duration<double>::zero())
            {

            }

            ~thread_meta_t()
            {
                
            }

            std::thread::native_handle_type get_native_handle()
            {
                return native_handle;
            }
            
            std::thread::id get_id() const
            {
                return id;
            }

            app_t::kind_t get_app() const
            {
                return app;
            }

			app_t::instance_id_t get_app_instance_id() const
            {
                return app_instance_id;
            }

            const ustring_t & get_name() const
            {
                return name;
            }

			std::chrono::duration<double> get_total_time() const
			{
                std::lock_guard<std::recursive_mutex> guard(mutex);
                if(flags.is_flag(flag_t::concluded))
                    return concluded_time - creation_time;

				return std::chrono::steady_clock::now() - creation_time;
			}

            bool are_stats_available() const
            {
                return thread_stats.get();
            }

            std::chrono::duration<double> get_total_busy_time() const
            {
                std::lock_guard<std::recursive_mutex> guard(mutex);
                if(flags.is_flag(flag_t::concluded))
                    return total_busy_time;

                if(thread_stats)
                    return thread_stats->get_total_busy_time();
                else
                    return std::chrono::duration<double>::zero();
            }

            double get_current_cpu_usage() const
            { 
                std::lock_guard<std::recursive_mutex> guard(mutex);
                if(flags.is_flag(flag_t::concluded))
                    return 0.0;

                if(thread_stats)
                     return thread_stats->get_current_cpu_usage();
                else
                    return 0.0;
            }

        private:

            enum flag_t
            {
                concluded,
                count
            };

            flags_t<flag_t> flags;

            app_t::kind_t app;
            app_t::instance_id_t app_instance_id;
            ustring_t name;
            mutable std::recursive_mutex mutex;
            std::thread::id id;
            std::thread::native_handle_type native_handle;
			std::chrono::steady_clock::time_point creation_time;
			std::chrono::steady_clock::time_point concluded_time;
            std::chrono::duration<double> total_busy_time;
            std::unique_ptr<thread_stats_t> thread_stats;

            friend class thread_t;

            void conclude()
            {
                std::lock_guard<std::recursive_mutex> guard(mutex);
                total_busy_time = get_total_busy_time();
                concluded_time = std::chrono::steady_clock::now();
                flags.set_flag(flag_t::concluded, true);
            }

    };

}

#endif