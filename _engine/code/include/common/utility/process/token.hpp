#ifndef ENGINE_COMMON_UTILITY_PROCESS_TOKEN_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_TOKEN_HPP

#pragma once

#include "utility/pattern/flags.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/text/ustring.hpp"
#include <functional>

namespace engine
{

	namespace process
	{
		class runner_base_t;

		class token_t
		{

		public:

			token_t(runner_base_t * runner) : runner(runner)
			{

			}

			bool is_requested_shutdown();

		private:

			runner_base_t * runner;

		};
	}
}

#endif