#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_DUMMY_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_DUMMY_HPP
#pragma once

#include "global/component/thread_accounter.hpp"

namespace engine
{

	class thread_accounter_dummy_t : public thread_accounter_t
	{

	public:

		threads_meta_t get_running_threads() final
		{
			threads_meta_t ret;
			return ret;
		}

	private:

	};
}

#endif