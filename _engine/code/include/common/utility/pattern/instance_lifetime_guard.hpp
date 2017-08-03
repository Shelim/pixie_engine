#ifndef ENGINE_COMMON_UTILITY_PATTERN_INSTANCE_GUARD_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_INSTANCE_GUARD_HPP
#pragma once

#include <memory>

namespace engine
{

	template<class object_t> class instance_lifetime_guard final
	{

	public:

		instance_lifetime_guard()
		{

		}

		void set_instance(std::unique_ptr<object_t> object)
		{
			instance = std::move(object);
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