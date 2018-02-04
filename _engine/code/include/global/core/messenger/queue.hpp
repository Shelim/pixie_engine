#ifndef ENGINE_COMMON_UTILITY_MESSENGER_QUEUE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_QUEUE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include "global/core/messenger/msg.hpp"
#include "global/core/messenger/instance.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/pattern/factory.hpp"
#include "global/core/process/process.hpp"
#include "global/core/process/runner/spawn.hpp"
#include "global/core/process/task.hpp"

namespace engine
{

    namespace messenger
    {
        template<class msg_actual_t, bool keep_history> class queue_history_t { };
        
        template<class msg_actual_t> class queue_history_t<msg_actual_t, false>
        {

        public:

            void keep(std::shared_ptr<msg_actual_t> msg)
            {
                // Do nothing, this is intended
            }
            void write(instance_t<msg_actual_t> * instance)
            {
                // Do nothing, this is intended
            }

        private:

        };

        template<class msg_actual_t> class queue_history_t<msg_actual_t, true>
        {

        public:

            void keep(std::shared_ptr<msg_actual_t> msg)
            {
                std::lock_guard<std::mutex> guard(mutex);
                msgs.push_back(std::move(msg));
            }
            void write(instance_t<msg_actual_t> * instance)
            {
                std::lock_guard<std::mutex> guard(mutex);
                for(auto & iter : msgs)
                {
                    instance->write(iter);
                }
            }

        private:

            std::vector<std::shared_ptr<msg_actual_t> > msgs;
            std::mutex mutex;

        };

        template <class queue_t, class msg_actual_t, bool is_async, bool keep_history> class queue_base_t
        {
        public:
            queue_base_t(std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner)
            {
                
            }
         };

        template<class queue_t, class msg_actual_t, bool keep_history> class queue_base_t<queue_t, msg_actual_t, false, keep_history>
        {

        public:

            queue_base_t(std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner) : runner_spawner(runner_spawner)
            {
                
            }

            virtual ~queue_base_t()
            {
                for(auto & instance : instances)
                    instance->unregister_remotely();
                instances.clear();
            }
            
            void write(std::shared_ptr<msg_actual_t> msg)
            {
                std::lock_guard<std::mutex> guard(mutex);

                for(auto & iter : instances)
                {
                    iter->write(msg);
                }
                history.keep(msg);
            }

            std::unique_ptr<instance_t<msg_actual_t> > register_callback(callback_t<msg_actual_t> callback, history_t history = history_t::skip)
            {
                std::lock_guard<std::mutex> guard(mutex);

                std::unique_ptr<instance_t<msg_actual_t> > instance = std::unique_ptr<instance_t<msg_actual_t> >(new instance_t<msg_actual_t>(static_cast<queue_t*>(this), callback, runner_spawner));
                instances.push_back(instance.get());

                if(history == history_t::dump_if_available)
                    this->history.write(instance.get());

                return std::move(instance);
            }

        private:

            void unregister_instance(instance_t<msg_actual_t> * instance)
            {
                std::lock_guard<std::mutex> guard(mutex);

                auto iter = std::find(instances.begin(), instances.end(), instance);
                if(iter != instances.end())
                    instances.erase(iter);
            }

            template<class T> friend class instance_t;

            std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner;

            queue_history_t<msg_actual_t, keep_history> history;
            std::vector<instance_t<msg_actual_t> *> instances;
            std::mutex mutex;

        };
        
        template<class queue_t, class msg_actual_t, bool keep_history> class queue_base_t<queue_t, msg_actual_t, true, keep_history>
        {

        public:

            queue_base_t(std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner) : id(next_id++), runner_spawner(runner_spawner)
            {
                runner = runner_spawner->create();
                runner->add_task(std::make_unique<task_func_t>([this](process::token_t*){ return execute(); }, format_string("Messenger '#1#' queue ###2#"_u, get_msg_type<msg_actual_t>(), id)));
            }

            virtual ~queue_base_t()
            {
                std::lock_guard<std::mutex> guard(mutex);
                for(auto & instance : instances)
                    instance->unregister_remotely();
                instances.clear();
                typename msg_internal_register_instance<msg_actual_t>::info_t info; 
                queue_msg.push(nullptr);
            }
            
            void write(std::shared_ptr<msg_actual_t> msg)
            {
                if(msg)
                {
                    std::lock_guard<std::mutex> guard(mutex);
                    queue_msg.push(std::make_unique<msg_t<msg_actual_t>>(msg));
                }
            }

            std::unique_ptr<instance_t<msg_actual_t> > register_callback(callback_t<msg_actual_t> callback, history_t history = history_t::skip)
            {
                std::lock_guard<std::mutex> guard(mutex);
                typename msg_internal_register_instance<msg_actual_t>::info_t info; 
                queue_msg.push(std::make_unique<msg_internal_register_instance<msg_actual_t>>(callback, history, &info));
                return std::move(info.wait_for_completion());
            }

        private:

            void unregister_instance(instance_t<msg_actual_t> * instance)
            {
                std::lock_guard<std::mutex> guard(mutex);
                typename msg_internal_unregister_instance<msg_actual_t>::info_t info; 
                queue_msg.push(std::make_unique<msg_internal_unregister_instance<msg_actual_t>>(instance, &info));
                info.wait_for_completion();
            }

            template<class T> friend class instance_t;

            std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner;

            queue_history_t<msg_actual_t, keep_history> history;
            std::vector<instance_t<msg_actual_t> *> instances;
            concurrent_queue_t<std::unique_ptr<msg_base_t>> queue_msg;
            std::unique_ptr<process::runner_spawn_t> runner;
            std::mutex mutex;
            signal_t signal;
            
            static uint32_t next_id;
            uint32_t id;
            
            task_base_t::result_t execute()
            {
                signal_t::guard_t guard(&signal);

                for (;;)
                {
                    std::unique_ptr<msg_base_t> msg_instance = std::move(queue_msg.pop());
                    msg_base_t * msg = msg_instance.get();
                    if(!msg)
                    {
                        return task_base_t::result_t::completed;
                    }
                    else if(msg->get_type() == msg_base_t::type_t::msg)
                    {
                        auto actual_msg = static_cast<msg_t<msg_actual_t>*>(msg)->get_actual();
                        for(auto & iter : instances)
                        {
                            iter->write(actual_msg);
                        }
                        history.keep(actual_msg);
                    }
                    else if(msg->get_type() == msg_base_t::type_t::register_instance)
                    {
                        auto register_message = static_cast<msg_internal_register_instance<msg_actual_t>*>(msg);
                        
                        std::unique_ptr<instance_t<msg_actual_t> > instance = std::unique_ptr<instance_t<msg_actual_t> >(new instance_t<msg_actual_t>(static_cast<queue_t*>(this), register_message->get_callback(), runner_spawner));
                        instances.push_back(instance.get());

                        if(register_message->get_history() == history_t::dump_if_available)
                            this->history.write(instance.get());

                        register_message->raport_completion(std::move(instance));
                    }
                    else if(msg->get_type() == msg_base_t::type_t::unregister_instance)
                    {
                        auto unregister_message = static_cast<msg_internal_unregister_instance<msg_actual_t>*>(msg);
                        
                        auto iter = std::find(instances.begin(), instances.end(), unregister_message->get_instance());
                        if(iter != instances.end())
                            instances.erase(iter);
                        
                        unregister_message->raport_completion();
                    }
                }
            }

        };

        template <class queue_t, class msg_actual_t, bool keep_history> uint32_t queue_base_t<queue_t, msg_actual_t, true, keep_history>::next_id = 1;

        template<class msg_actual_t> class queue_t : public queue_base_t<queue_t<msg_actual_t>, msg_actual_t, msg_actual_t::is_queue_async, msg_actual_t::keep_history>
        {
            public:
                queue_t(std::shared_ptr<ifactory<process::runner_spawn_t> > runner_spawner) : queue_base_t<queue_t<msg_actual_t>, msg_actual_t, msg_actual_t::is_queue_async, msg_actual_t::keep_history>(runner_spawner) {}

        };

    }

}

#endif