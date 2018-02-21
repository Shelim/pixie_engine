#ifndef ENGINE_GLOBAL_COMPONENT_THREAD_ACCOUNTER_HPP
#define ENGINE_GLOBAL_COMPONENT_THREAD_ACCOUNTER_HPP
#pragma once

#include <chrono>
#include "global/core/app.hpp"
#include "global/core/thread.hpp"

namespace engine
{

    class thread_accounter_t
    {

    public:

        virtual ~thread_accounter_t()
        {

        }

        class thread_info_t
        {

            public:

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
                    return total_time;
                }

                std::chrono::duration<double> get_cpu_usage_in_last_second() const
                {
                    return cpu_usage_in_last_second;
                }

                thread_info_t(std::thread::id id, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::chrono::duration<double> total_time, std::chrono::duration<double> cpu_usage_in_last_second) :
                    id(id), app(app), app_instance_id(app_instance_id), name(name), total_time(total_time), cpu_usage_in_last_second(cpu_usage_in_last_second)
                {

                }

            private:

                std::thread::id id;
                app_t::kind_t app;
                app_t::instance_id_t app_instance_id;
                ustring_t name;
                std::chrono::duration<double> total_time;
                std::chrono::duration<double> cpu_usage_in_last_second;
        };

        typedef std::vector<thread_info_t> threads_info_t;

        virtual threads_info_t get_snapshot() = 0;

    };

SETTINGS_TABLE_START(thread_accounter_t)
    SETTINGS_TABLE_ENTRY(std::chrono::duration<double>, snapshot_refresh_interval)
SETTINGS_TABLE_END()

}

#include "global/component/thread_accounter/dummy.hpp"
#include "global/component/thread_accounter/real.hpp"

#endif