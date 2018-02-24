#ifndef ENGINE_GLOBAL_COMPONENT_PROFILER_REAL_HPP
#define ENGINE_GLOBAL_COMPONENT_PROFILER_REAL_HPP
#pragma once

#include "global/component/profiler.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{
	typedef profiler_t profiler_provider_base_t;

	REGISTER_PROVIDER_BASE_TYPE(profiler_t, profiler_provider_base_t)

    class profiler_real_t : public profiler_t
    {

    public:

        profiler_real_t(std::unique_ptr<holder_t<profiler_t> > profiler)  : profiler(std::move(profiler))
        {

        }

        ~profiler_real_t()
        {

        }

        void prof_begin_section(const char * name) final
        {
            profiler->get_provider()->prof_begin_section(name);
        }
        
        void prof_end_section() final
        {
            profiler->get_provider()->prof_end_section();
        }

        void name_current_thread(const ustring_t & name) final
        {
            profiler->get_provider()->name_current_thread(name);
        }

    private:

        std::unique_ptr<holder_t<profiler_t> > profiler;

    };

}

#include "global/component/profiler/provider/remotery.hpp"

#endif