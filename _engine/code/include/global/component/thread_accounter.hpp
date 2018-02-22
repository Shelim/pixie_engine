#ifndef ENGINE_GLOBAL_COMPONENT_THREAD_ACCOUNTER_HPP
#define ENGINE_GLOBAL_COMPONENT_THREAD_ACCOUNTER_HPP
#pragma once

#include <chrono>
#include "global/core/app.hpp"
#include "global/core/thread/thread.hpp"

namespace engine
{

    class thread_accounter_t
    {

    public:

        virtual ~thread_accounter_t()
        {

        }

        typedef std::vector<std::shared_ptr<thread_meta_t> > threads_meta_t;

        virtual threads_meta_t get_running_threads() = 0;

    };

}

#include "global/component/thread_accounter/dummy.hpp"
#include "global/component/thread_accounter/real.hpp"

#endif