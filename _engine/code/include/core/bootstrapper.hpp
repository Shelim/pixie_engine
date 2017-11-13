#ifndef ENGINE_COMMON_BOOTSTRAPPER_HPP
#define ENGINE_COMMON_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include <functional>
#include "boost/di.hpp"
#include "core/manifest/app.hpp"
#include "utility/pattern/instance_lifetime_guard.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/pattern/factory.hpp"
#include "component/config.hpp"
#include "component/config_monitor.hpp"
#include "component/config_storage.hpp"
#include "component/data_archives.hpp"
#include "component/data_creator.hpp"
#include "component/data_monitor.hpp"
#include "component/data_source.hpp"
#include "component/environment_info.hpp"
#include "component/filesystem.hpp"
#include "component/logger.hpp"
#include "component/path_resolver.hpp"
#include "component/renderer_status.hpp"
#include "component/terminal.hpp"

#define INTERNAL_PROVIDER_INIT(provider_type, ...) engine::register_providers_for<engine::provider_type##_t 
#define INTERNAL_PROVIDER_0(provider_type) 
#define INTERNAL_PROVIDER_1(provider_type, p1) , engine::provider_type##_provider_##p1##_t
#define INTERNAL_PROVIDER_2(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_1(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_3(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_2(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_4(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_3(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_5(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_4(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_6(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_5(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_7(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_6(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_8(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_7(provider_type, __VA_ARGS__))
#define INTERNAL_PROVIDER_9(provider_type, p1, ...) INTERNAL_PROVIDER_1(provider_type, p1) EXPAND(INTERNAL_PROVIDER_8(provider_type, __VA_ARGS__))

#define EXPAND( x ) x
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N

#define ENABLE_COMPONENT(component) engine::register_as<engine::component##_real_t, engine::component##_t>
#define MOCKUP_COMPONENT(component) engine::register_as<engine::component##_mockup_t, engine::component##_t>
#define USE_PROVIDER_FOR(provider_type, provider) engine::register_provider_for<engine::provider_type##_t INTERNAL_PROVIDER_1(provider_type, provider)>
#define USE_PROVIDERS_FOR(...) EXPAND(INTERNAL_PROVIDER_INIT(__VA_ARGS__)) EXPAND(EXPAND(_GET_NTH_ARG(__VA_ARGS__, INTERNAL_PROVIDER_9, INTERNAL_PROVIDER_8, INTERNAL_PROVIDER_7, INTERNAL_PROVIDER_6, INTERNAL_PROVIDER_5, INTERNAL_PROVIDER_4, INTERNAL_PROVIDER_3, INTERNAL_PROVIDER_2, INTERNAL_PROVIDER_1, INTERNAL_PROVIDER_0))(__VA_ARGS__)) >
#define APP(app_id) engine::app<engine::value_of(engine::manifest_app_t::app_t::app_id)>

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

	template<class owner_t, class settings_t> class settings_for
	{

	};

	template<std::underlying_type<manifest_app_t::app_t>::type app_id> class app
	{
		
	};

	template<class ... registerable_t> class bootstrapper_t final
	{

	private:


		class scoped_scope {
		public:
			template <class, class T>
			class scope {
			public:
				template <class T_>
				using is_referable = typename boost::di::wrappers::shared<scoped_scope, T>::template is_referable<T_>;

				template <class, class, class TProvider, class T_ = boost::di::aux::decay_t<decltype(boost::di::aux::declval<TProvider>().get())>>
				static decltype(boost::di::wrappers::shared<scoped_scope, T_>{
					std::shared_ptr<T_>{std::shared_ptr<T_>{boost::di::aux::declval<TProvider>().get()}}})
					try_create(const TProvider &);

					template <class T_, class, class TProvider>
					auto create(const TProvider &provider) {
						return create_impl<boost::di::aux::decay_t<decltype(provider.get())>>(provider);
					}

					scope() = default;
					scope(scope &&other) noexcept : object_(other.object_) { other.object_ = nullptr; }
					~scope() noexcept { delete object_; }

			private:
				template <class, class TProvider>
				auto create_impl(const TProvider &provider) {
					if (!object_) {
						object_ = new std::shared_ptr<T>{ provider.get() };
					}
					return boost::di::wrappers::shared<scoped_scope, T, std::shared_ptr<T> &>{*object_};
				}

				std::shared_ptr<T> *object_ = nullptr;
			};
		};

		static constexpr scoped_scope scoped{};

		template<class ...registerable_t> struct obj
		{

		};

		static auto provide_di(obj<>)
		{
			return boost::di::make_injector(
#define ENGINE_COMPONENT_STD(component)	boost::di::bind<component##_t>().in(scoped).to <component##_dummy_t>(),
#include "def/component.def"
				boost::di::make_injector()
			);
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
			return boost::di::make_injector(boost::di::bind<component_t>().in(scoped).to <implementation_t>()[boost::di::override]);
		}

		template<class owner_t, class settings_values_t> static auto provide_di_unit(settings_for<owner_t, settings_values_t>)
		{
			return boost::di::make_injector(boost::di::bind<settings_t<owner_t> >().to <settings_implementation_t<owner_t, settings_values_t> >());
		}

		template<std::underlying_type<manifest_app_t::app_t>::type app_id> static auto provide_di_unit(app<app_id>)
		{
			return boost::di::make_injector(boost::di::bind<manifest_app_t::app_t>().to(static_cast<manifest_app_t::app_t>(app_id)));
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
			std::shared_ptr<component_t> ptr = injector.create<std::shared_ptr<component_t> >();
			component_t * ret = ptr.get();
			guard.set_instance(ptr);
			return ret;
		}

		template<class component_t> bootstrapper_t & construct_component_with_lifetime_guard(component_t* & ret, instance_lifetime_guard<component_t> & guard)
		{
			std::shared_ptr<component_t> ptr = injector.create<std::shared_ptr<component_t> >();
			ret = ptr.get();
			guard.set_instance(std::move(ptr));

			return (*this);
		}


	private:

	};



	template<class ... registerable_t> constexpr typename bootstrapper_t<registerable_t...>::scoped_scope bootstrapper_t<registerable_t...>::scoped;

}

#endif