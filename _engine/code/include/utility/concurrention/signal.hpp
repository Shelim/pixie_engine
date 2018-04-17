#ifndef ENGINE_COMMON_UTILITY_CONCURRENTION_SIGNAL_HPP
#define ENGINE_COMMON_UTILITY_CONCURRENTION_SIGNAL_HPP

#include <queue>
#include <mutex>
#include <memory>
#include <array>
#include <thread>

#pragma once

namespace engine
{
    class signal_t
    {
        
    public:
        
        signal_t() : released(false)
        {
            
        }

        ~signal_t()
        {
            signal();
        }

        bool is_released() const
        {
            std::lock_guard<std::mutex> guard(mutex);
            return released;
        }

        void wait()
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(!released)
            {
                condition_variable.wait(lock, [this]{return released;});
            }
        }
        
        void signal()
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(!released)
            {
                released = true;
                condition_variable.notify_all();
            }
        }

        class guard_t
        {

        public:

            guard_t(signal_t * signal) : signal(signal)
            {

            }

            ~guard_t()
            {
                signal->signal();
            }

        private:

            signal_t * signal;

        };

    private:

        bool released;
        std::condition_variable condition_variable;
        mutable std::mutex mutex;

    };

    class signaler_t;

    class signal_waiter_t
    {
        
    public:

        void wait_for_signal()
        {
            internal->wait();
        }

    private:

        std::shared_ptr<signal_t> internal;
        signal_waiter_t(std::shared_ptr<signal_t> internal) : internal(internal)
        {

        }

        friend class signaler_t;

    };

    class signaler_t
    {

    public:

        signaler_t()
        {
            internal = std::make_shared<signal_t>();
        }

        ~signaler_t()
        {
            signal_ready();
        }

        void signal_ready()
        {
            internal->signal();
        }
        
        signal_waiter_t get_waiter()
        {
            return signal_waiter_t(internal);
        }
        
    private:

        std::shared_ptr<signal_t> internal;

    };
}

#endif