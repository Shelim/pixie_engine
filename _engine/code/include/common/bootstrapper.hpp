#ifndef ENGINE_COMMON_BOOTSTRAPPER_HPP
#define ENGINE_COMMON_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include <functional>
#include "boost/di.hpp"
#include "utility/pattern/instance_lifetime_guard.hpp"
#include "utility/pattern/component.hpp"

namespace engine
{
	
	class bootstrapper_t final
	{

	public:

		bootstrapper_t()
		{

		}

		~bootstrapper_t()
		{

		}

		template<class component_t, class component_implementation_t> bootstrapper_t & register_component()
		{

			return (*this);
		}

		template<class owner_t, class... providers_t> bootstrapper_t & register_providers_for()
		{

			return (*this);
		}

		template<class ... components_t> bootstrapper_t & use_dummy_for()
		{

			return (*this);
		}

		bootstrapper_t & complete_configuration()
		{

			return (*this);
		}

		template<class component_t> std::shared_ptr<component_t> construct_component()
		{

			return (*this);
		}

		template<class component_t> bootstrapper_t & construct_component(std::shared_ptr<component_t> & ptr)
		{

			return (*this);
		}

		template<class component_t> component_t* construct_component_with_lifetime_guard(instance_lifetime_guard<component_t> & guard)
		{

			return (*this);
		}

		template<class component_t> bootstrapper_t & construct_component_with_lifetime_guard(component_t* & ptr, instance_lifetime_guard<component_t> & guard)
		{

			return (*this);
		}


	private:

	};

}

#endif