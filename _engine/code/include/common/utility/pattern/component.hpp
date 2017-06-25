#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <memory>

namespace game
{

	template<class component_t> class dummy_for final
	{
		static_assert("This component does not supports dummy for!");

		typedef component_t type;
	};

	class provider_t
	{

	public:

		virtual ~provider_t()
		{
			
		}

	private:

	};

	template<class ... providers_t> class holder_t final
	{

	public:

		holder_t(std::unique_ptr<provider_t>... providers) : provider(std::move(providers)...)
		{

		}

	private:

		std::tuple<std::unique_ptr<providers_t>...> providers;
	};

	class component_base_t
	{

	public:

		virtual ~component_base_t()
		{

		}

	};
	
	template<class ... providers_t> class component_t : public component_base_t
	{

	public:

	protected:

		auto get_holder()
		{
			return &holder;
		}

	private:

		holder_t<providers_t...> holder;

	};

}

#endif