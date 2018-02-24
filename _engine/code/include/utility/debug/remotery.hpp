#ifndef ENGINE_COMMON_UTILITY_DEBUG_REMOTERY_HPP
#define ENGINE_COMMON_UTILITY_DEBUG_REMOTERY_HPP
#pragma once

#include <remotery.h>
#include "utility/text/ustring.hpp"
#include <string>
#include <set>

namespace engine
{
    class remotery_t
    {

    public:

        remotery_t() : rmt(nullptr)
        {
            rmtSettings* settings = rmt_Settings();

            settings->limit_connections_to_localhost = true;

            rmtError ret_code = rmt_CreateGlobalInstance(&rmt);
        }

        ~remotery_t()
        {
            rmt_DestroyGlobalInstance(rmt);
        }

        void prof_begin_section(const char * name)
        {
            rmt_BeginCPUSampleDynamic(name, RMTSF_None);
        }
        
        void prof_end_section()
        {
            rmt_EndCPUSample();
        }

        void name_current_thread(const ustring_t & name)
        {
            rmt_SetCurrentThreadName(name.get_cstring());
        }

    private:

        Remotery* rmt;

    };

}

#endif