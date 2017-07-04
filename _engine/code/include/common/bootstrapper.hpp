#ifndef ENGINE_COMMON_BOOTSTRAPPER_HPP
#define ENGINE_COMMON_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include <functional>
#include "boost/di.hpp"
#include "utility/pattern/instance_lifetime_guard.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{

	template<class owner_t, class... providers_t> class register_providers_for
	{

	};

	template<class owner_t, class provider_t> class register_provider_for
	{

	};

	template<class implementation_t, class component_t> class register_as
	{

	};
	
	template<class ... registerable_t> class bootstrapper_t final
	{

	private:


		template<class ...registerable_t> struct obj
		{

		};

		static auto provide_di(obj<>)
		{
			return boost::di::make_injector();
		}

		template<class owner_t, class... providers_t> static auto provide_di_unit(register_providers_for<owner_t, providers_t...>)
		{
			return boost::di::make_injector(boost::di::bind<holder_t<owner_t>>().to < holder_implementation_t<owner_t, providers_t...> >());
		}

		template<class owner_t, class provider_t> static auto provide_di_unit(register_provider_for<owner_t, provider_t>)
		{
			return boost::di::make_injector(boost::di::bind<holder_t<owner_t>>().to < holder_implementation_t<owner_t, provider_t> >());
		}

		template<class component_t, class implementation_t> static auto provide_di_unit(register_as<implementation_t, component_t>)
		{
			return boost::di::make_injector(boost::di::bind<component_t>().to <implementation_t>());
		}

		template<class provider_for, class... registerable_t> static auto provide_di(obj<provider_for, registerable_t...>)
		{
			return boost::di::make_injector(provide_di_unit(provider_for{}), provide_di(obj<registerable_t...>{}));
		};

		static auto provide_final_di()
		{
			return provide_di(obj<registerable_t...>{});
		}

		decltype(provide_final_di()) injector;

	public:

		bootstrapper_t() : injector(provide_final_di())
		{

		}

		~bootstrapper_t()
		{

		}


		template<class component_t> std::shared_ptr<component_t> construct_component()
		{
			return injector.create<std::shared_ptr<component_t> >();
		}

		template<class component_t> bootstrapper_t & construct_component(std::shared_ptr<component_t> & ptr)
		{
			ptr = injector.create<std::shared_ptr<component_t> >();
			return (*this);
		}

		template<class component_t> component_t* construct_component_with_lifetime_guard(instance_lifetime_guard<component_t> & guard)
		{
			std::unique_ptr<component_t> ptr = injector.create<std::unique_ptr<component_t> >();
			component_t * ret = ptr.get();
			guard.set_instance(std::move(ptr));
			return ret;
		}

		template<class component_t> bootstrapper_t & construct_component_with_lifetime_guard(component_t* & ret, instance_lifetime_guard<component_t> & guard)
		{
			std::unique_ptr<component_t> ptr = injector.create<std::unique_ptr<component_t> >();
			ret = ptr.get();
			guard.set_instance(std::move(ptr));

			return (*this);
		}


	private:

	};

}

#endif