#ifndef ENGINE_GLOBAL_COMPONENT_PROFILER_PROVIDER_REMOTERY_HPP
#define ENGINE_GLOBAL_COMPONENT_PROFILER_PROVIDER_REMOTERY_HPP
#pragma once

#include "global/component/profiler/real.hpp"
#include "global/core/messenger/messenger.hpp"
#include "utility/debug/remotery.hpp"
#include <string>
#include <set>

namespace engine
{
    class profiler_provider_remotery_t : public profiler_provider_base_t
    {

    public:

        profiler_provider_remotery_t(std::shared_ptr<remotery_t> remotery) : remotery(remotery)
        {

        }

        ~profiler_provider_remotery_t()
        {
            
        }

        void prof_begin_section(const char * name) final
        {
            remotery->prof_begin_section(name);
        }
        
        void prof_end_section() final
        {
            remotery->prof_end_section();
        }

        void name_current_thread(const ustring_t & name)
        {
            remotery->name_current_thread(name);
        }

    private:

        std::shared_ptr<remotery_t> remotery;

    };

}

#include "global/component/profiler/provider/remotery.hpp"

#endif