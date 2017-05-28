#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_COLLECTION_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_COLLECTION_HPP
#pragma once

#include "common/logger_output/provider_console.hpp"
#include "common/logger_output/provider_data.hpp"
#include "common/logger_output/provider_raw.hpp"
#include "common/config.hpp"

#if PIXIE_OUTPUT_TYPE != PIXIE_OUTPUT_TYPE_game && PIXIE_OUTPUT_TYPE != PIXIE_OUTPUT_TYPE_editor && PIXIE_OUTPUT_TYPE != PIXIE_OUTPUT_TYPE_launcher && PIXIE_OUTPUT_TYPE != PIXIE_OUTPUT_TYPE_dll && PIXIE_OUTPUT_TYPE != PIXIE_OUTPUT_TYPE_test
#error "Specify log provider for this pixie output type!"
#endif

namespace engine
{
	class logger_t;
	class config_t;
	
	namespace logger_output
	{
		class providers_t final
		{

		public:

			providers_t
			(
#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t > provider_##logger_output_var,
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t > provider_##logger_output_var,
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t > provider_##logger_output_var,
#endif
#include "common/std/logger_output_std.hpp"
				std::shared_ptr<config_t> config
			) :
#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var(provider_##logger_output_var),
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var(provider_##logger_output_var),
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var(provider_##logger_output_var),
#endif
#include "common/std/logger_output_std.hpp"
				config(config)
			{

			}

			void force_resave()
			{
#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var->force_resave();
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var->force_resave();
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) provider_##logger_output_var->force_resave();
#endif
#include "common/std/logger_output_std.hpp"

			}

#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> get_provider_##logger_output_var() { return provider_##logger_output_var; };
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> get_provider_##logger_output_var() { return provider_##logger_output_var; };
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> get_provider_##logger_output_var() { return provider_##logger_output_var; };
#endif
#include "common/std/logger_output_std.hpp"

			void update()
			{
#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)if(config->get<bool>(engine::config_t::item_t::game_logger_output_##logger_output_var)) provider_##logger_output_var->enable(); else provider_##logger_output_var->disable();
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)if(config->get<bool>(engine::config_t::item_t::editor_logger_output_##logger_output_var)) provider_##logger_output_var->enable(); else provider_##logger_output_var->disable();
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)if(config->get<bool>(engine::config_t::item_t::launcher_logger_output_##logger_output_var)) provider_##logger_output_var->enable(); else provider_##logger_output_var->disable();
#endif
#include "common/std/logger_output_std.hpp"
			}

			void on_item_changed(const std::size_t item_changed)
			{

#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_changed(item_changed);
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_changed(item_changed);
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_changed(item_changed);
#endif
#include "common/std/logger_output_std.hpp"
			}

			void on_item_append(const logger_t::item_t & item)
			{

#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_append(item);
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_append(item);
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)provider_##logger_output_var->on_item_append(item);
#endif
#include "common/std/logger_output_std.hpp"
			}

		private:

#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game
#define GAME_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> provider_##logger_output_var;
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor
#define EDITOR_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> provider_##logger_output_var;
#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher
#define LAUNCHER_LOGGER_OUTPUT_STD(logger_output_var, u1, u2)std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> provider_##logger_output_var;
#endif
#include "common/std/logger_output_std.hpp"

			std::shared_ptr<config_t> config;

		};

	}

}

#endif