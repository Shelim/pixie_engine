#ifndef ENGINE_GLOBAL_CORE_BOOTSTRAPPER_HPP
#define ENGINE_GLOBAL_CORE_BOOTSTRAPPER_HPP
#pragma once

#include <memory>
#include <functional>
#include <fstream>
#include "boost/di.hpp"
#include "boost/di/extension/scopes/shared.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/pattern/factory.hpp"
#include "global/component/app_accounter.hpp"
#include "global/component/config.hpp"
#include "global/component/config_common/changed_provider.hpp"
#include "global/component/config_monitor.hpp"
#include "global/component/config_storage.hpp"
#include "global/component/environment_info.hpp"
#include "global/component/filesystem.hpp"
#include "global/component/logger.hpp"
#include "global/component/profiler.hpp"
#include "global/component/thread_accounter.hpp"
#include "local/core/manifest/app.hpp"
#include "local/component/renderer_status.hpp"
#include "local/component/terminal.hpp"
#include "settings/default_config.hpp"
#include "global/core/app/context.hpp"
#include "global/core/program.hpp"
#include "global/core/policy/instances_application.hpp"
#include "global/core/policy/instances_program.hpp"
#include "global/core/policy/renderer_thread.hpp"
#include "global/core/program.hpp"

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
   
	namespace
	{

		static constexpr shared_scope shared{};


		auto injector_disabled = [] {
			return boost::di::make_injector(
#define ENGINE_COMPONENT_IMPL(component)	boost::di::bind<component##_t>().in(shared).to <component##_dummy_t>(),
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
#include "def/component.def"
				boost::di::make_injector());
		};
		auto injector_mockup = [] {
			return boost::di::make_injector(
#define ENGINE_COMPONENT_IMPL(component)	boost::di::bind<component##_t>().in(shared).to <component##_mockup_t>(),
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
//#include "def/component.def"
				boost::di::make_injector());
		};
		auto injector_enabled = [] {
			return boost::di::make_injector(
#define ENGINE_COMPONENT_IMPL(component)	boost::di::bind<component##_t>().in(shared).to <component##_real_t>(),
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
#include "def/component.def"
				boost::di::make_injector());
		};

		auto injector_factorable = [] {
			return boost::di::make_injector(
#define ENGINE_FACTORABLE_IMPL(factorable) boost::di::bind<ifactory<factorable>>().to(factory<factorable>{}),
#define ENGINE_FACTORABLE_DEF(...) DEFINE_TYPE_PASS(ENGINE_FACTORABLE_IMPL, __VA_ARGS__)
#include "def/factorable.def"
				boost::di::make_injector());
		};

		auto injector_context = [] (app_context_t* context){
			return boost::di::make_injector(
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) boost::di::bind<engine::component##_t>().to(context->get_program()->get_##component()),
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
				boost::di::bind<app_t::instance_id_t>().named(engine::instance_id).to(context->get_instance_id()),
				boost::di::bind<>().to(context));
		};

		class boostrapper_t
		{

		public:

			class unused_t
			{

			};

			boostrapper_t(boost::di::injector<
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t>,
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_COMPONENT_IMPL(component) std::shared_ptr<component##_t>,
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
#include "def/component.def"
			unused_t> injector) : injector(std::move(injector))
			{

			}
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<engine::component##_t> get_##component() { return injector.create<std::shared_ptr<engine::component##_t> >(); }
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_COMPONENT_IMPL(component) std::shared_ptr<engine::component##_t> get_##component() { return injector.create<std::shared_ptr<engine::component##_t> >(); }
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
#include "def/component.def"

		private:
		
			boost::di::injector<
#define ENGINE_GLOBAL_COMPONENT_IMPL(component) std::shared_ptr<component##_t>,
#define ENGINE_GLOBAL_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_GLOBAL_COMPONENT_IMPL, __VA_ARGS__)
#include "def/global_component.def"
#define ENGINE_COMPONENT_IMPL(component) std::shared_ptr<component##_t>,
#define ENGINE_COMPONENT_DEF(...) DEFINE_TYPE_PASS(ENGINE_COMPONENT_IMPL, __VA_ARGS__)
#include "def/component.def"
			unused_t> injector;

		};
	}

}


#define INTERNAL_PROVIDER_INIT(provider_type, ...) boost::di::bind<engine::holder_t<engine::provider_type##_t>>().to < engine::holder_implementation_t<engine::provider_type##_t 

#define BEGIN_BOOTSTRAPPER(name, context) auto name = engine::boostrapper_t(std::move(boost::di::make_injector(engine::injector_context(context), 
#define ALL_COMPONENTS_BY_DEFAULT_ARE(type) engine::injector_##type(),
#define ENABLE_COMPONENT(component) boost::di::bind<engine::component##_t>().in(engine::shared).to <engine::component##_real_t>()[boost::di::override],
#define MOCKUP_COMPONENT(component) boost::di::bind<engine::component##_t>().in(engine::shared).to <engine::component##_mockup_t>()[boost::di::override],
#define DISABLE_COMPONENT(component) boost::di::bind<engine::component##_t>().in(engine::shared).to <engine::component##_dummy_t>()[boost::di::override],
#define USE_SETTINGS(owner, id) boost::di::bind<engine::settings_t<engine::owner> >().to <engine::settings_implementation_t<engine::owner, engine::settings_metadata_##owner##_##id##_t> >(),
#define USE_PROVIDER_FOR(provider_type, provider) boost::di::bind<engine::holder_t<engine::provider_type##_t>>().to < engine::holder_implementation_t<engine::provider_type##_t INTERNAL_PROVIDER_1(provider_type, provider)> >(),
#define USE_PROVIDERS_FOR(...) EXPAND(INTERNAL_PROVIDER_INIT(__VA_ARGS__)) EXPAND(EXPAND(_GET_NTH_ARG(__VA_ARGS__, INTERNAL_PROVIDER_9, INTERNAL_PROVIDER_8, INTERNAL_PROVIDER_7, INTERNAL_PROVIDER_6, INTERNAL_PROVIDER_5, INTERNAL_PROVIDER_4, INTERNAL_PROVIDER_3, INTERNAL_PROVIDER_2, INTERNAL_PROVIDER_1, INTERNAL_PROVIDER_0))(__VA_ARGS__)) > >(),
#define APP(app_id) boost::di::bind<engine::app_t::kind_t>().to(engine::app_t::kind_t::app_id),
#define END_BOOTSTRAPPER() engine::injector_factorable() )));

#include "global/core/app/bootstrapper_standard.hpp"

#endif