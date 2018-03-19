#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "global/component/config_common/enum.hpp"
#include "global/core/app.hpp"

namespace engine
{
	
    namespace messenger
    {

        class msg_config_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = false;
			static constexpr bool keep_history = false;

#define ENGINE_CONFIG_CHANGE_SOURCE(...) DEFINE_ENUM_ONLY_1ST_TYPE(source_t, __VA_ARGS__)
#define ENGINE_CONFIG_TYPE(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/config.def"

            type_t get_type() const
            {
                return type;
            }

            source_t get_source() const
            {
                return source;
            }

            struct change_global_t
            {
                engine::config_global_t global;
            };
            struct change_app_specific_t
            {
                engine::config_app_specific_t app_specific;
            };
            struct change_local_t
            {
                engine::app_t::kind_t app;
                engine::config_local_t local;
            };

            msg_config_t(source_t source, engine::config_global_t global) : type(type_t::global), source(source)
            {
                value.global.global = global;
            }

            msg_config_t(source_t source, engine::config_app_specific_t app_specific) : type(type_t::app_specific), source(source)
            {
                value.app_specific.app_specific = app_specific;
            }
            
            msg_config_t(source_t source, engine::app_t::kind_t app, engine::config_local_t local) : type(type_t::local), source(source)
            {
                value.local.app = app;
                value.local.local = local;
            }

            const change_global_t * get_global() const
            {
                if(type != type_t::global) return nullptr;
                return &(value.global);
            }

            const change_app_specific_t * get_app_specific() const
            {
                if(type != type_t::app_specific) return nullptr;
                return &(value.app_specific);
            }

            const change_local_t * get_local() const
            {
                if(type != type_t::local) return nullptr;
                return &(value.local);
            }

            bool has_changed(engine::config_global_t global)
            {
                return (type == type_t::global && value.global.global == global);
            }

            bool has_changed(engine::config_app_specific_t app_specific)
            {
                return (type == type_t::app_specific && value.app_specific.app_specific == app_specific);
            }

            bool has_changed(engine::app_t::kind_t app, engine::config_local_t local)
            {
                return (type == type_t::local && value.local.app == app && value.local.local == local);
            }

        private:

            type_t type;
            source_t source;

            union
            {
                change_global_t global;
                change_app_specific_t app_specific;
                change_local_t local;
            } value;

        };

    }

}


#define ENGINE_CONFIG_CHANGE_SOURCE(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::messenger::msg_config_t::source_t, __VA_ARGS__)
#define ENGINE_CONFIG_TYPE(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::messenger::msg_config_t::type_t, __VA_ARGS__)
#include "def/config.def"

#endif