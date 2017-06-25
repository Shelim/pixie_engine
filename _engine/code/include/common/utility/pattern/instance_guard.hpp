#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <memory>

namespace game
{

	template<class object_t> class instance_guard final
	{

	public:

		void set_instance(object_t * object)
		{
			instance = object;
		}

		void destroy_instance()
		{
			instance.reset();
		}

	private:

		std::unique_ptr<object_t> instance;

	};

}

#endif