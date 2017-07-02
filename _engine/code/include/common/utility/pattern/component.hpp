#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <memory>

namespace game
{

	template<class component_t> class component_base_t
	{

	public:

		virtual ~component_base_t()
		{

		}

	};
	
	template<class owner_t> class provider_base_t
	{

	public:

		virtual ~provider_base_t()
		{
			
		}

	private:

	};

	template<class owner_t> class holder_base_t
	{

	public:

		virtual ~holder_base_t()
		{

		}

		virtual provider_base_t<owner_t> * get_provider() = 0;

	};

	template<class owner_t, class ... providers_t> class holder_t final : public holder_base_t<owner_t>
	{

	public:

		holder_t(std::unique_ptr<providers_t>... providers) : providers(std::move(providers)...)
		{

		}

		provider_base_t<owner_t> * get_provider() final
		{
			return std::get<0>(providers).get();
		}

	private:

		std::tuple<std::unique_ptr<providers_t>...> providers;
	};
	
}

#endif