
#ifndef ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_HPP
#define ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_HPP
#pragma once

#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "utility/pattern/class_settings.hpp"
#include "global/component/config_common/enum.hpp"
#include "global/core/messenger/msg/config.hpp"

namespace engine
{

	class config_changed_provider_base_t
	{

	public:

		virtual ~config_changed_provider_base_t()
		{

		}

        virtual void notify_on_change(engine::config_global_t global, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) = 0;
        virtual void notify_on_change(engine::config_app_specific_t app_specific, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) = 0;
        virtual void notify_on_change(engine::app_t::kind_t app, engine::config_local_t local, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) = 0;

	};

}

#include "global/component/config_common/changed_provider/logger.hpp"
#include "global/component/config_common/changed_provider/messenger.hpp"
#include "global/component/config_common/changed_provider/mockup.hpp"

#endif