#ifndef ENGINE_GLOBAL_CORE_THREAD_HPP
#define ENGINE_GLOBAL_CORE_THREAD_HPP
#pragma once

#include "utility/pattern/fourcc.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/accountable/accountable.hpp"
#include "global/core/app.hpp"
#include <thread>
#include <functional>
#include <memory>

namespace engine
{

    class thread_t
    {

        public:

            thread_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, std::shared_ptr<holder_t<accountable_thread_t> > notifier, const ustring_t & name, std::function<void()> thread_func) :
                app(app), app_instance_id(app_instance_id), name(name)
            {
                thread = std::thread([this, notifier, thread_func]{ id = std::this_thread::get_id();
                instance = std::make_unique<accountable_thread_instance_t>(notifier, this);
                thread_func(); } );
            }

            ~thread_t()
            {
                try
                {
                    if(thread.joinable())
                        thread.join();
                }
	            catch (const std::system_error& e) {}
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

        private:

            std::thread thread;
            std::thread::id id;
            app_t::kind_t app;
            app_t::instance_id_t app_instance_id;
            ustring_t name;
            std::unique_ptr<accountable_thread_instance_t > instance;
    };

    class thread_factory_t
    {

        public:

            thread_factory_t(std::shared_ptr<holder_t<accountable_thread_t> > notifier) : notifier(notifier)
            {

            }

            std::unique_ptr<thread_t> create(const ustring_t & name, std::function<void()> thread_func)
            {
                return std::make_unique<thread_t>(app_t::kind_t::_engine, 0, notifier, name, thread_func);
            }

            std::unique_ptr<thread_t> create(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::function<void()> thread_func)
            {
                return std::make_unique<thread_t>(app, app_instance_id, notifier, name, thread_func);
            }

        private:

            std::shared_ptr<holder_t<accountable_thread_t> > notifier;

    };

}

#endif