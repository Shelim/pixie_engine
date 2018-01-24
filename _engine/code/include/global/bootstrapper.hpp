#ifndef ENGINE_GLOBAL_BOOTSTRAPPER_HPP
#define ENGINE_GLOBAL_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include <functional>
#include <fstream>
#include "boost/di.hpp"
#include "core/manifest/app.hpp"
#include "core/monitor/monitors.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/pattern/factory.hpp"
#include "component/config.hpp"
#include "component/config_common/changed_provider.hpp"
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
#include "settings/default_config.hpp"
#include "global/core/app/context.hpp"

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

////////////////////////////////////////////////////////////////////////////////////

namespace engine
{

    namespace global
    {        
	
		namespace
		{
			
			class shared_scope {
			public:
				template <class, class T>
				class scope {
				public:
					template <class T_>
					using is_referable = typename boost::di::wrappers::shared<shared_scope, T>::template is_referable<T_>;

					template <class, class, class TProvider>
					static auto try_create(const TProvider& provider)
						-> decltype(boost::di::wrappers::shared<shared_scope, T>{std::shared_ptr<T>{provider.get()}});

					template <class, class, class TProvider>
					auto create(const TProvider& provider) {
						if (!object_) {
							object_ = std::shared_ptr<T>{ provider.get() };
						}
						return boost::di::wrappers::shared<shared_scope, T>{object_};
					}

				private:
					std::shared_ptr<T> object_;
				};
			};

			static constexpr shared_scope shared{};


			auto injector_dummies = [] {
				return boost::di::make_injector(
	#define ENGINE_COMPONENT_DEF(component)	boost::di::bind<component##_t>().in(shared).to <component##_dummy_t>(),
	#include "def/component.def"
	#define ENGINE_FACTORABLE_DEF(factorable) boost::di::bind<ifactory<factorable>>().to(factory<factorable>{}),
	#include "def/factorable.def"
					boost::di::make_injector());
			};

			auto injector_context = [] (global::app_context_t* context){ // ToDo: register instances for global components
				return boost::di::make_injector(
	/*
	#define ENGINE_COMPONENT_DEF(component)	boost::di::bind<component##_t>().in(shared).to <component##_dummy_t>(),
	#include "def/component.def"
	*/
					boost::di::make_injector());
			};
		}

	}
}

#define INTERNAL_PROVIDER_INIT(provider_type, ...) boost::di::bind<engine::holder_t<engine::provider_type##_t>>().to < engine::holder_implementation_t<engine::provider_type##_t 

#define BEGIN_BOOTSTRAPPER(name, context) const auto name = boost::di::make_injector(engine::global::injector_context(context.get()), 
#define ENABLE_COMPONENT(component) boost::di::bind<engine::component##_t>().in(engine::global::shared).to <engine::component##_real_t>()[boost::di::override],
#define MOCKUP_COMPONENT(component) boost::di::bind<engine::component##_t>().in(engine::global::shared).to <engine::component##_mockup_t>()[boost::di::override],
#define USE_SETTINGS(owner, id) boost::di::bind<engine::settings_t<engine::owner> >().to <engine::settings_implementation_t<engine::owner, engine::settings_metadata_##owner##_##id##_t> >(),
#define USE_PROVIDER_FOR(provider_type, provider) boost::di::bind<engine::holder_t<engine::provider_type##_t>>().to < engine::holder_implementation_t<engine::provider_type##_t INTERNAL_PROVIDER_1(provider_type, provider)> >(),
#define USE_PROVIDERS_FOR(...) EXPAND(INTERNAL_PROVIDER_INIT(__VA_ARGS__)) EXPAND(EXPAND(_GET_NTH_ARG(__VA_ARGS__, INTERNAL_PROVIDER_9, INTERNAL_PROVIDER_8, INTERNAL_PROVIDER_7, INTERNAL_PROVIDER_6, INTERNAL_PROVIDER_5, INTERNAL_PROVIDER_4, INTERNAL_PROVIDER_3, INTERNAL_PROVIDER_2, INTERNAL_PROVIDER_1, INTERNAL_PROVIDER_0))(__VA_ARGS__)) > >(),
#define APP(app_id) boost::di::bind<engine::manifest_app_t::app_t>().to(engine::manifest_app_t::app_t::app_id),
#define END_BOOTSTRAPPER() engine::global::injector_dummies() );

#define GET_COMPONENT(bootstrapper, component) bootstrapper.create<std::shared_ptr<engine::component##_t> >()

#endif