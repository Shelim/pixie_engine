#ifndef ENGINE_GLOBAL_CORE_THREAD_THREAD_HPP
#define ENGINE_GLOBAL_CORE_THREAD_THREAD_HPP
#pragma once

#include "utility/pattern/fourcc.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/thread/meta.hpp"
#include "global/core/accountable.hpp"
#include "global/core/app.hpp"
#include "global/component/profiler.hpp"
#include <thread>
#include <functional>
#include <memory>

namespace engine
{

    class thread_t
    {

        public:

            thread_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, std::shared_ptr<holder_t<accountable_thread_t> > notifier, const ustring_t & name, std::function<void()> thread_func, std::shared_ptr<holder_t<thread_stats_t> > stats_provider, std::shared_ptr<profiler_t> profiler) : profiler(profiler)
            {
                thread = std::thread([=]{ profiler->name_current_thread(name); profiler->prof_begin_section("Thread initialization"); profiler->prof_end_section(); thread_func(); } );
                thread_meta = std::make_shared<thread_meta_t>(app, app_instance_id, name, thread.get_id(), thread.native_handle(), stats_provider->get_provider()->get_stats_for_thread(thread.get_id(), thread.native_handle()));
                instance = std::make_unique<accountable_thread_instance_t>(notifier, this);

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

            std::shared_ptr<thread_meta_t> get_thread_meta()
            {
                return thread_meta;
            }

        private:

            std::thread thread;
            std::unique_ptr<accountable_thread_instance_t > instance;
            std::shared_ptr<thread_meta_t> thread_meta;
            std::shared_ptr<profiler_t> profiler;

    };

    class thread_factory_t
    {

        public:

            thread_factory_t(std::shared_ptr<holder_t<accountable_thread_t> > notifier, std::shared_ptr<holder_t<thread_stats_t> > stats_provider, std::shared_ptr<profiler_t> profiler) : notifier(notifier), stats_provider(stats_provider), profiler(profiler)
            {

            }

            std::unique_ptr<thread_t> create(const ustring_t & name, std::function<void()> thread_func)
            {
                return std::make_unique<thread_t>(app_t::kind_t::_engine, 0, notifier, name, thread_func, stats_provider, profiler);
            }

            std::unique_ptr<thread_t> create(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::function<void()> thread_func)
            {
                return std::make_unique<thread_t>(app, app_instance_id, notifier, name, thread_func, stats_provider, profiler);
            }

        private:

            std::shared_ptr<holder_t<accountable_thread_t> > notifier;
            std::shared_ptr<holder_t<thread_stats_t> > stats_provider;
            std::shared_ptr<profiler_t> profiler;

    };

}

#endif