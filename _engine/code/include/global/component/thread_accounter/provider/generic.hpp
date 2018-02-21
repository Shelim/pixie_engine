#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_GENERIC_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_GENERIC_HPP
#pragma once

#include "global/component/thread_accounter/real.hpp"

namespace engine
{

    class thread_accounter_provider_generic_t : public thread_accounter_provider_base_t
	{

    public:

    private:

        class thread_info_generic_t : public thread_info_t
        {

        public:

            thread_info_generic_t(thread_t * thread) : thread_info_t(thread)
            {

            }

            std::chrono::duration<double> get_cpu_usage_in_last_second() const final
            {
                return std::chrono::duration<double>::zero();
            }

        };

		std::unique_ptr<thread_info_t> create_thread_info(thread_t * thread) final
        {
            return std::make_unique<thread_info_generic_t>(thread);
        }

    };

}

#endif