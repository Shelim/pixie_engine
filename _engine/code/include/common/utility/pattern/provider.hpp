#ifndef ENGINE_COMMON_UTILITY_PATTERN_PROVIDER_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_PROVIDER_HPP
#pragma once

#include <array>
#include <memory>

namespace engine
{

	template<class owner_t> struct provider_info_t;

#define REGISTER_PROVIDER_BASE_TYPE(owner, provider_base) template<> struct ::engine::provider_info_t<owner> { typedef provider_base provider_t; };
		
	template<class owner_t> class holder_t
	{

	public:

		virtual ~holder_t()
		{

		}
		
		virtual typename provider_info_t<owner_t>::provider_t * get_provider(std::size_t id = 0) = 0;
		virtual std::size_t get_providers_count() const = 0;

	};

	template<class owner_t, class ... providers_t> class holder_implementation_t : public holder_t<owner_t>
	{

	public:

		holder_implementation_t(std::unique_ptr<providers_t>... providers) : providers{ {std::move(providers)...} }
		{

		}
		
		typename provider_info_t<owner_t>::provider_t * get_provider(std::size_t id = 0) final
		{
			return providers[id].get();
		}
	
		std::size_t get_providers_count() const final
		{
			return sizeof...(providers_t);
		}

	private:

		std::array<std::unique_ptr<typename provider_info_t<owner_t>::provider_t >, sizeof...(providers_t)> providers;
	};
	
}

#endif