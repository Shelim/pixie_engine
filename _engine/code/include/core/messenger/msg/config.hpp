#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "component/config_common/enum.hpp"
#include "core/manifest/app.hpp"

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

#include "def/enum/config_msg.def"

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
                engine::manifest_app_t::app_t app;
                engine::config_local_t local;
            };

            msg_config_t(engine::manifest_app_t::app_t local_app, source_t source, engine::config_global_t global) : local_app(local_app), type(type_t::global), source(source)
            {
                value.global.global = global;
            }

            msg_config_t(engine::manifest_app_t::app_t local_app, source_t source, engine::config_app_specific_t app_specific) : local_app(local_app), type(type_t::app_specific), source(source)
            {
                value.app_specific.app_specific = app_specific;
            }
            
            msg_config_t(engine::manifest_app_t::app_t local_app, source_t source, engine::manifest_app_t::app_t app, engine::config_local_t local) : local_app(local_app), type(type_t::local), source(source)
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

            bool has_changed(engine::config_local_t local)
            {
                return (type == type_t::local && value.local.app == local_app && value.local.local == local);
            }

            bool has_changed(engine::manifest_app_t::app_t app, engine::config_local_t local)
            {
                return (type == type_t::local && value.local.app == app && value.local.local == local);
            }

        private:

            engine::manifest_app_t::app_t local_app;
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

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/config_msg.def"
#include "core/utility/enum_to_string.hpp"

#endif