#ifndef ENGINE_GLOBAL_COMPONENT_PROFILER_HPP
#define ENGINE_GLOBAL_COMPONENT_PROFILER_HPP
#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{

    class profiler_t
    {

    public:

        virtual ~profiler_t()
        {

        }

        virtual void prof_begin_section(const char * name)
        {

        }

        virtual void prof_end_section()
        {
            
        }
        
        virtual void name_current_thread(const ustring_t & name)
        {

        }

        class section_t
        {

        public:

            section_t(std::shared_ptr<profiler_t> profiler, const char * name) : profiler(profiler)
            {
                profiler->prof_begin_section(name);
            }

            ~section_t()
            {
                profiler->prof_end_section();
            }

        private:

            std::shared_ptr<profiler_t> profiler;

        };

    };

}

#define prof_function(profiler) engine::profiler_t::section_t _function_section_at_##__LINE__(profiler, __FUNCTION__)

#include "global/component/profiler/dummy.hpp"
#include "global/component/profiler/real.hpp"

#endif