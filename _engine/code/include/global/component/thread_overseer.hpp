#ifndef ENGINE_GLOBAL_COMPONENT_THREAD_OVERSEER_HPP
#define ENGINE_GLOBAL_COMPONENT_THREAD_OVERSEER_HPP
#pragma once

#include <chrono>
#include "global/core/app.hpp"
#include "global/core/thread.hpp"

namespace engine
{

    class thread_overseer_t
    {

    public:

        virtual ~thread_overseer_t()
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

                std::chrono::seconds get_total_time() const
                {
                    return total_time;
                }

                std::chrono::seconds get_snapshot_time() const
                {
                    return snapshot_time;
                }

                thread_info_t(std::thread::id id, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::chrono::seconds total_time, std::chrono::seconds snapshot_time) :
                    id(id), app(app), app_instance_id(app_instance_id), name(name), total_time(total_time), snapshot_time(snapshot_time)
                {

                }

            private:

                std::thread::id id;
                app_t::kind_t app;
                app_t::instance_id_t app_instance_id;
                ustring_t name;
                std::chrono::seconds total_time;
                std::chrono::seconds snapshot_time;
        };

        virtual std::vector<thread_info_t> snapshot_state()
        {

        }

    };

}

#include "global/component/thread_overseer/dummy.hpp"
#include "global/component/thread_overseer/real.hpp"

#endif