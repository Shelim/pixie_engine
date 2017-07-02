#ifndef ENGINE_COMMON_BOOTSTRAPPER_HPP
#define ENGINE_COMMON_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include "boost/di.hpp"
#include "utility/pattern/instance_guard.hpp"

namespace game
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

		template<class component_t, class... providers_t> bootstrapper_t & register_providers_for()
		{

		}

		template<class component_t, class... providers_t> bootstrapper_t & register_providers_for()
		{

		}

		template<class ... components_t> bootstrapper_t & use_dummy_for()
		{

		}

		bootstrapper_t & complete_configuration()
		{

		}
		
		template<class component_t> std::shared_ptr<component_t> construct_component()
		{

		}

		template<class component_t> bootstrapper_t & construct_component(std::shared_ptr<component_t> & ptr)
		{

		}

		template<class component_t> component_t* construct_component_with_guard(instance_guard<component_t> & guard)
		{

		}

		template<class component_t> bootstrapper_t & construct_component_with_guard(component_t* & ptr, instance_guard<component_t> & guard)
		{

		}


	private:


	};

}

#endif