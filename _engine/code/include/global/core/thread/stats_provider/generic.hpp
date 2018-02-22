#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_GENERIC_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_GENERIC_HPP
#pragma once

#include "global/core/thread/stats_provider.hpp"

#include <windows.h>

namespace engine
{

    class thread_stats_provider_generic_t : public thread_stats_provider_base_t
	{

    public:

        std::unique_ptr<thread_stats_t> get_stats_for_thread(std::thread::id id, std::thread::native_handle_type native_handle) final
        {
            return std::unique_ptr<thread_stats_t>(nullptr);
        }

    };

}

#endif