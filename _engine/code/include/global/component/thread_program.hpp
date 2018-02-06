#ifndef ENGINE_GLOBAL_COMPONENT_THREAD_PROGRAM_HPP
#define ENGINE_GLOBAL_COMPONENT_THREAD_PROGRAM_HPP
#pragma once

#include "global/core/app.hpp"
#include "global/core/process/runner/program_thread.hpp"

namespace engine
{

    class thread_program_t
    {

    public:

        virtual ~thread_program_t()
        {

        }

//      virtual std::shared_ptr<runner_base_t> get_thread(engine::process::runner_thread_t thread) = 0;

    };

}

#include "global/component/thread_program/dummy.hpp"
#include "global/component/thread_program/real.hpp"

#endif

/*
    
#ifndef ENGINE_GLOBAL_THREAD_HPP
#define ENGINE_GLOBAL_THREAD_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class global_thread_t // For example: renderer thread!
    {

        public:

#include "def/enum/global_thread.def"

            class instance_t
            {

            };

            typedef std::function<void()> callback_t;

            std::unique_ptr<instance_t> register_callback(callback_t callback);

            void call_callbacks_once();

        private:

    };

     class global_threads_t
     {

        public:
        
            global_thread_t * get_global_thread(global_thread_t::id_t id)
            {
                return &global_threads[value_of(id)];
            }

        private:

            std::array<global_thread_t, value_of(global_thread_t::id_t::count)> global_threads;

     };
}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/global_thread.def"
#include "global/core/utility/enum_to_string.hpp"

#endif

*/