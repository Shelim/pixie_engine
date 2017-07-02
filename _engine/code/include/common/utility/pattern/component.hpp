#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <array>
#include <memory>

namespace engine
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

	template<class owner_t> class holder_t
	{

	public:

		virtual ~holder_t()
		{

		}

		virtual provider_base_t<owner_t> * get_provider(std::size_t id = 0) = 0;
		virtual std::size_t get_providers_count() const = 0;

	};

	template<class owner_t, class ... providers_t> class holder_implementation_t final : public holder_t<owner_t>
	{

	public:

		holder_implementation_t(std::unique_ptr<providers_t>... providers) : providers(std::move(providers)...)
		{

		}

		provider_base_t<owner_t> * get_provider(std::size_t id = 0) final
		{
			return providers[id].get();
		}
		
		std::size_t get_providers_count() const final
		{
			return sizeof...(providers_t);
		}

	private:

		std::array<std::unique_ptr<provider_base_t<owner_t> >, sizeof...(providers_t)> providers;
	};
	
}

#endif