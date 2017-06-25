#include "common/config.hpp"
#include "common/platform.hpp"
#include "common/manifest_app.hpp"
#include "common/config_io/provider_base.hpp"
#include "common/data/database_providers.hpp"
#include "common/data/database_items.hpp"
#include <fstream>
#include <istream>
#include <ostream>

namespace engine
{
	template<class archive_t> void serialize(archive_t & ar, engine::config_t::data_t & data)
	{
#define GAME_CONFIG_STD(name, valr, vald) ar(cereal::make_nvp(#name, data[static_cast<std::size_t>(engine::config_t::item_t::name)]));
#include "common/std/config_std.hpp"
	}
}


engine::config_t::config_t(std::shared_ptr<engine::logger_t> logger, std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::manifest_app_t> manifest_appmanifest) : logger(logger), platform(platform), manifest_app(manifest_app)
{
	set_initial();
}

void engine::config_t::resave()
{
	auto provider_shared = provider.lock();
	if (provider_shared)
	{
		provider_shared->resave();
	}
}
/*
void engine::config_t::reload(engine::data::database_providers_t * database_providers)
{
	std::lock_guard<std::recursive_mutex> guard(mutex);

	auto provider = database_providers->get()->get_provider(path);
	
	if (!provider)
	{
		logger->p_err(_U("Config file not found"));
		revert_to_default();
		return;
	}

	auto input = provider->construct_input();
	engine::data::input_streambuf_t buff(input.get());

	std::istream is(&buff);

	
	try
	{
		XMLPixieInputArchive archive(is);
		archive(cereal::make_nvp("config", data));
	}
	catch (cereal::Exception & ex)
	{
		logger->p_err(_U("Parsing failed: #1#"), ustring_t::from_utf8(ex.what()));
		logger->p_warn(_U("Configuration file '#1#' corrupted, all values will be reverted to default values"), path);
		revert_to_default();
		return;
	}

#define GAME_CONFIG_STD(item, unused1, unused2) if (is_updated(item_t::item)) logger->p_msg(_U("Config item '#1#' updated '#2#' -> '#3#'"), _U(#item), data_prev[static_cast<std::size_t>(item_t::item)], data[static_cast<std::size_t>(item_t::item)]);
#include "common/std/config_std.hpp"

}

void engine::config_t::resave(engine::data::database_providers_t * database_providers)
{
	std::lock_guard<std::recursive_mutex> guard(mutex);

	auto output = database_providers->write_or_create(path);

	engine::data::output_streambuf_t streambuf(output.get());

	std::ostream os(&streambuf);
	{
		XMLPixieOutputArchive archive(os, engine::XMLPixieOutputArchive::Options::WithXsl("./engine/config/config.xsl"));
		const ustring_t game_name = ustring_t::from_utf8(manifest_app->game_name());
		archive(cereal::make_nvp("game_name", game_name));
		archive(cereal::make_nvp("config", data));
	}
}

void engine::config_t::update_from_database(engine::data::database_providers_t * database_providers)
{
	data_prev = data;

	if (database_providers->is_type_change(virtual_path_t::type_t::config))
	{
		reload(database_providers);
	}
	if (is_dirty)
	{
		resave(database_providers);
		is_dirty = false;
	}
}
*/

engine::config_t::~config_t()
{
	set(item_t::CONFIG_CURRENT_APP(is_first_run), false);
}


engine::ustring_t engine::config_t::def(item_t key)
{
	switch (key)
	{
#if PIXIE_IS_DEBUG_BUILD
#define GAME_CONFIG_STD(name, valr, vald) case item_t::name: return to_string(vald);
#else
#define GAME_CONFIG_STD(name, valr, vald) case item_t::name: return to_string(valr);
#endif
#include "common/std/config_std.hpp"
	}

	return _U("");
}
