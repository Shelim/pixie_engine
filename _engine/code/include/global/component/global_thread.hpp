#ifndef ENGINE_GLOBAL_GLOBAL_THREAD_HPP
#define ENGINE_GLOBAL_GLOBAL_THREAD_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class global_thread_t
        {

        public:

            virtual ~global_thread_t()
            {

            }

        };

    }

}

#include "global/component/global_thread/dummy.hpp"
#include "global/component/global_thread/real.hpp"

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
#include "core/utility/enum_to_string.hpp"

#endif

*/