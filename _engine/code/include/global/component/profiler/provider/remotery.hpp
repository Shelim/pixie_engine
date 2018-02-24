#ifndef ENGINE_GLOBAL_COMPONENT_PROFILER_PROVIDER_REMOTERY_HPP
#define ENGINE_GLOBAL_COMPONENT_PROFILER_PROVIDER_REMOTERY_HPP
#pragma once

#include "global/component/profiler/real.hpp"
#include "global/core/messenger/messenger.hpp"
#include <remotery.h>
#include <string>
#include <set>

namespace engine
{
    class profiler_provider_remotery_t : public profiler_provider_base_t
    {

    public:

        profiler_provider_remotery_t() : rmt(nullptr)
        {
            rmtSettings* settings = rmt_Settings();

            settings->limit_connections_to_localhost = true;

            std::lock_guard<std::mutex> guard(mutex);
            rmtError ret_code = rmt_CreateGlobalInstance(&rmt);
        }

        ~profiler_provider_remotery_t()
        {
            std::lock_guard<std::mutex> guard(mutex);
            rmt_DestroyGlobalInstance(rmt);
        }

        void prof_begin_section(const char * name) final
        {
            std::lock_guard<std::mutex> guard(mutex);
            rmt_BeginCPUSampleDynamic(name, RMTSF_None);
        }
        
        void prof_end_section() final
        {
            std::lock_guard<std::mutex> guard(mutex);
            rmt_EndCPUSample();
        }

        void name_current_thread(const ustring_t & name)
        {
            std::lock_guard<std::mutex> guard(mutex);
            rmt_SetCurrentThreadName(name.get_cstring());
        }

    private:

        Remotery* rmt;
        std::mutex mutex;

    };

}

#include "global/component/profiler/provider/remotery.hpp"

#endif