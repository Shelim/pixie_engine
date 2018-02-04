#ifndef ENGINE_GLOBAL_CORE_IGNITER_HPP
#define ENGINE_GLOBAL_CORE_IGNITER_HPP
#pragma once

#include "global/core/bootstrapper.hpp"
#include "global/policy/instances_application.hpp"
#include "global/policy/instances_program.hpp"
#include "global/policy/renderer_thread.hpp"
#include "global/core/program.hpp"
#include <type_traits>

namespace engine
{

    namespace global
    {        

        template<class type_t, class implementation_t> class ignition_implementation_t
        {
            public:
              typedef std::false_type is_allowed_t;
        };

        template<class type_t, class... args_t> class ignition_implementation_real_t;

        template<class type_t> class igniter_platform_requires_something_that_is_forbidden_by_policy_t
        {

        };

        template<class type_t> class ignition_implementation_real_t<type_t>
        {
            public:
              typedef typename igniter_platform_requires_something_that_is_forbidden_by_policy_t<type_t> type;
        };
        
        template<class type_t, class implementation_t, class... implementations_t> class ignition_implementation_real_t<type_t, implementation_t, implementations_t...>
        {
            public:
              typedef typename std::conditional<ignition_implementation_t<type_t, implementation_t>::is_allowed_t::value, implementation_t, typename ignition_implementation_real_t<type_t, implementations_t...>::type>::type type;
        };

#define GAME_ALLOW_POLICY_DEF(item, value) template<> class ignition_implementation_t<engine::global::item##_t, engine::global::item##_##value##_t> { public: typedef std::true_type is_allowed_t; };
#include "def/global_policy.def"

#define IGNITER_IMPLEMENTATION_1(item, p1) , engine::global::item##_##p1##_t
#define IGNITER_IMPLEMENTATION_2(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_1(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_3(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_2(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_4(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_3(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_5(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_4(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_6(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_5(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_7(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_6(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_8(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_7(item, __VA_ARGS__))
#define IGNITER_IMPLEMENTATION_9(item, p1, ...) IGNITER_IMPLEMENTATION_1(item, p1) EXPAND(IGNITER_IMPLEMENTATION_8(item, __VA_ARGS__))

		auto injector_igniter_disabled = [] {
			return boost::di::make_injector(
#define ENGINE_GLOBAL_COMPONENT_DEF(component) boost::di::bind<component##_t>().in(shared).to<component##_dummy_t>(),
//#include "def/global_component.def"
				boost::di::make_injector());
		};
		auto injector_igniter_mockup = [] {
			return boost::di::make_injector(
#define ENGINE_GLOBAL_COMPONENT_DEF(component) boost::di::bind<component##_t>().in(shared).to<component##_mockup_t>(),
//#include "def/global_component.def"
				boost::di::make_injector());
		};
		auto injector_igniter_enabled = [] {
			return boost::di::make_injector(
#define ENGINE_GLOBAL_COMPONENT_DEF(component) boost::di::bind<component##_t>().in(shared).to<component##_real_t>(),
//#include "def/global_component.def"
				boost::di::make_injector());
		};

#define BEGIN_PLATFORM_CONFIGURATION(name) auto name = engine::global::igniter_t(boost::di::make_injector(
#define PLATFORM_ALLOWS_POLICIES(item, ...) boost::di::bind<engine::global::item##_t>().to<typename engine::global::ignition_implementation_real_t<engine::global::item##_t EXPAND(EXPAND(_GET_NTH_ARG(item, __VA_ARGS__, IGNITER_IMPLEMENTATION_9, IGNITER_IMPLEMENTATION_8, IGNITER_IMPLEMENTATION_7, IGNITER_IMPLEMENTATION_6, IGNITER_IMPLEMENTATION_5, IGNITER_IMPLEMENTATION_4, IGNITER_IMPLEMENTATION_3, IGNITER_IMPLEMENTATION_2, IGNITER_IMPLEMENTATION_1, IGNITER_IMPLEMENTATION_0))(item, __VA_ARGS__))>::type>(),
#define ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(type) engine::global::injector_igniter_##type(),
#define ENABLE_GLOBAL_COMPONENT(component) boost::di::bind<engine::global::component##_t>().in(engine::global::shared).to <engine::global::component##_real_t>()[boost::di::override],
#define MOCKUP_GLOBAL_COMPONENT(component) boost::di::bind<engine::global::component##_t>().in(engine::global::shared).to <engine::global::component##_mockup_t>()[boost::di::override],
#define DISABLE_GLOBAL_COMPONENT(component) boost::di::bind<engine::global::component##_t>().in(engine::global::shared).to <engine::global::component##_dummy_t>()[boost::di::override],
#define END_PLATFORM_CONFIGURATION() boost::di::make_injector() ));

        class igniter_t
        {

            public:

                igniter_t(boost::di::injector<
#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t>,
#include "def/global_component.def"
                program_t::unused_t> injector) : injector(std::move(injector))
                {

                }

                std::shared_ptr<program_t> ignite_from_main(int argc, char * argv[])
                {
                    return std::make_shared<program_t>(std::move(injector));
                }

            private:

                boost::di::injector<
#define ENGINE_GLOBAL_COMPONENT_DEF(component) std::shared_ptr<component##_t>,
#include "def/global_component.def"
                program_t::unused_t> injector;
        };

    }

}

#endif