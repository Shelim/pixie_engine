#ifndef ENGINE_COMMON_UTILITY_MESSENGER_INSTANCE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_INSTANCE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <functional>
#include "core/messenger/messenger.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/concurrention/signal.hpp"
#include "utility/pattern/factory.hpp"
#include "core/process/runner/spawn.hpp"
#include "core/messenger/msg.hpp"

namespace engine
{

    namespace messenger
    {        
        template<class msg_actual_t, bool is_async> class instance_base_t
        {
            
        };

        template<class msg_actual_t> class instance_base_t<msg_actual_t, false>
        {

        public:

			instance_base_t(callback_t<msg_actual_t> callback) : callback(callback)
			{

			}

			virtual ~instance_base_t()
			{

			}

            void write(std::shared_ptr<msg_actual_t> msg)
            {
                if(msg)
					callback(msg.get());
            }

		private:

			callback_t<msg_actual_t> callback;

        };

        template<class msg_actual_t> class instance_base_t<msg_actual_t, true>
        {

        public:

            instance_base_t(callback_t<msg_actual_t> callback, std::unique_ptr<process::runner_spawn_t> runner) : callback(callback), id(next_id++), runner(std::move(runner))
            {
                runner->add_task(std::make_unique<task_func_t>([this](process::token_t*){ return execute(); }, format_string("Messenger '#1#' output instance queue ###2#"_u, get_msg_type<msg_actual_t>(), id)));
            }

            virtual ~instance_base_t()
            {
                std::lock_guard<std::mutex> guard(mutex);
                flags.set_flag(flag_t::being_destroyed, true);
                queue_msg.push(nullptr);
                signal.wait();
            }

            void write(std::shared_ptr<msg_actual_t> msg)
            {
                std::lock_guard<std::mutex> guard(mutex);
                if(msg && !flags.is_flag(flag_t::being_destroyed))
                {
                    queue_msg.push(msg);
                }
            }

        private:

            enum class flag_t
            {
                being_destroyed,
                count
            };
    
            flags_t<flag_t> flags;
            std::mutex mutex;
			callback_t<msg_actual_t> callback;

            concurrent_queue_t<std::shared_ptr<msg_actual_t>> queue_msg;
            std::unique_ptr<process::runner_spawn_t> runner;

            static uint32_t next_id;
            uint32_t id;

            signal_t signal;

            task_base_t::result_t execute()
            {
                signal_t::guard_t guard(&signal);

                for (;;)
                {
                    std::shared_ptr<msg_actual_t> msg = queue_msg.pop();
                    if(!msg) return task_base_t::result_t::completed;
                    else callback(msg.get());
                }
            }
        };


        template<class msg_actual_t> uint32_t instance_base_t<msg_actual_t, true>::next_id = 1;
        template<class msg_actual_t> class queue_t;

        template<class msg_actual_t> class instance_t : public instance_base_t<msg_actual_t, msg_actual_t::is_instance_async>
        {

        public:

            ~instance_t()
            {
                if(queue_owner)
                    queue_owner->unregister_instance(this);
            }

        private:

            void unregister_remotely()
            {
                queue_owner = nullptr;
            }

            template<class T1, class T2, bool B1, bool B2> friend class queue_base_t;
            
            instance_t(queue_t<msg_actual_t> * queue_owner, callback_t<msg_actual_t> callback) : instance_base_t<msg_actual_t, msg_actual_t::is_instance_async>(callback), queue_owner(queue_owner)
            {

            }

            queue_t<msg_actual_t> * queue_owner;

        };

    }

}

#endif