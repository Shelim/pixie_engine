#ifndef ENGINE_COMMON_BOOTSTRAPPER_HPP
#define ENGINE_COMMON_BOOTSTRAPPER_HPP
#pragma once

#include <memory>

namespace game
{
	class engine_t;
	
	class bootstrapper_t final
	{

	public:

		bootstrapper_t()
		{

		}

		~bootstrapper_t()
		{

		}

		engine_t * get_engine()
		{
			return engine.get();
		}

		bootstrapper_t & configure_debug()
		{

		}

		bootstrapper_t & configure_release()
		{

		}

		template<class component_t, class provider_t> bootstrapper_t & register_provider_for()
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


	private:

		std::unique_ptr<engine_t> engine;

	};

}

#endif