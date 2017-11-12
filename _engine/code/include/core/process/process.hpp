#ifndef ENGINE_COMMON_UTILITY_PROCESS_PROCESS_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_PROCESS_HPP

#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{

	class process_base_t
	{

	public:

		process_base_t()
		{
			
		}
		
		virtual ~process_base_t()
		{

		}

		virtual ustring_t get_name() const
		{
			return "Generic process"_u;
		}

	private:

	};

}

#endif