
#ifndef ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_MESSENGER_CONFIG_HPP
#define ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_MESSENGER_CONFIG_HPP
#pragma once

#include "component/logger.hpp"
#include "component/config_common/changed_provider.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "core/manifest/app.hpp"
#include "utility/pattern/class_settings.hpp"
#include "component/config_common/enum.hpp"
#include "core/messenger/messenger.hpp"
#include "core/messenger/msg/config.hpp"

namespace engine
{

	class config_changed_provider_messenger_t : public config_changed_provider_base_t
	{

	public:

        config_changed_provider_messenger_t(std::shared_ptr<messenger_config_t> messenger_config, std::shared_ptr<manifest_app_t> manifest_app) : messenger_config(messenger_config), manifest_app(manifest_app)
        {

        }

        void notify_on_change(engine::config_global_t global, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			messenger_config->write(std::make_shared<messenger::msg_config_t>(manifest_app->get_local_app(), source, global));
        }

        void notify_on_change(engine::config_app_specific_t app_specific, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			messenger_config->write(std::make_shared<messenger::msg_config_t>(manifest_app->get_local_app(), source, app_specific));
        }

        void notify_on_change(engine::manifest_app_t::app_t app, engine::config_local_t local, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			messenger_config->write(std::make_shared<messenger::msg_config_t>(manifest_app->get_local_app(), source, app, local));
        }

    private:

        std::shared_ptr<messenger_config_t> messenger_config;
        std::shared_ptr<manifest_app_t> manifest_app;

	};

}

#endif