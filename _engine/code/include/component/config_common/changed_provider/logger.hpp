
#ifndef ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_LOGGER_HPP
#define ENGINE_COMPONENT_CONFIG_COMMON_CHANGED_PROVIDER_LOGGER_HPP
#pragma once

#include "component/logger.hpp"
#include "component/config_common/changed_provider.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "core/manifest/app.hpp"
#include "utility/pattern/class_settings.hpp"
#include "component/config_common/enum.hpp"
#include "core/messenger/msg/config.hpp"

namespace engine
{

	class config_changed_provider_logger_t : public config_changed_provider_base_t
	{

	public:

        config_changed_provider_logger_t(std::shared_ptr<logger_t> logger) : logger(logger)
        {

        }

        void notify_on_change(engine::config_global_t global, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			logger->log_msg(config, "Global setting '#1#' was just changed to '#2#' (triggered by #3# change)!"_u, global, val, source);
        }

        void notify_on_change(engine::config_app_specific_t app_specific, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			logger->log_msg(config, "App-specific setting '#1#' was just changed to '#2#' (triggered by #3# change)!"_u, app_specific, val, source);
        }

        void notify_on_change(engine::manifest_app_t::app_t app, engine::config_local_t local, ustring_t val, engine::messenger::msg_config_t::source_t source = engine::messenger::msg_config_t::source_t::internal) final
        {
			logger->log_msg(config, "Local setting '#1#' for app '#4#' was just changed to '#2#' (triggered by #3# change)!"_u, local, val, source, app);
        }

    private:

        std::shared_ptr<logger_t> logger;

	};

}

#endif