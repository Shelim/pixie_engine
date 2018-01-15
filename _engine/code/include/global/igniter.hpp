#ifndef ENGINE_GLOBAL_IGNITER_HPP
#define ENGINE_GLOBAL_IGNITER_HPP
#pragma once

#include "core/manifest/os.hpp"
#include "global/bootstrapper.hpp"
#include "global/component.hpp"
#include "global/policy.hpp"
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

#define INTERNAL_IMPLEMENTATION_1(item, p1) , engine::global::item##_##p1##_t
#define INTERNAL_IMPLEMENTATION_2(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_1(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_3(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_2(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_4(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_3(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_5(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_4(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_6(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_5(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_7(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_6(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_8(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_7(item, __VA_ARGS__))
#define INTERNAL_IMPLEMENTATION_9(item, p1, ...) INTERNAL_IMPLEMENTATION_1(item, p1) EXPAND(INTERNAL_IMPLEMENTATION_8(item, __VA_ARGS__))

		auto injector_igniter = [] {
			return boost::di::make_injector(
/*
#define ENGINE_GLOBAL_COMPONENT_DEF(component) boost::di::bind<ifactory<singleton##_t>>().to(factory<singleton##_t>{}),
#include "def/global_component.def"
*/
				boost::di::make_injector());
		};

#define BEGIN_PLATFORM_POLICY_CONFIGURATION() namespace engine { namespace global { auto instance = [] { return boost::di::make_injector(
#define PLATFORM_ALLOWS_POLICIES(item, ...) boost::di::bind<engine::global::item##_t>().to<typename engine::global::ignition_implementation_real_t<engine::global::item##_t EXPAND(EXPAND(_GET_NTH_ARG(item, __VA_ARGS__, INTERNAL_IMPLEMENTATION_9, INTERNAL_IMPLEMENTATION_8, INTERNAL_IMPLEMENTATION_7, INTERNAL_IMPLEMENTATION_6, INTERNAL_IMPLEMENTATION_5, INTERNAL_IMPLEMENTATION_4, INTERNAL_IMPLEMENTATION_3, INTERNAL_IMPLEMENTATION_2, INTERNAL_IMPLEMENTATION_1, INTERNAL_IMPLEMENTATION_0))(item, __VA_ARGS__))>::type>(),
#define END_PLATFORM_POLICY_CONFIGURATION() injector_igniter() ); }; } }

        class igniter_t
        {

            public:

                std::unique_ptr<program_t> ignite_from_main(int argc, char * argv[])
                {
                    return std::make_unique<program_t>();
                }

        };

    }

}

#endif