#include "component/config.hpp"
#include "utility/messenger/msg_config_updated.hpp"
#include "platform/config_storage.hpp"


namespace engine
{
#define GAME_CONFIG_GLOBAL_STD(type_t, name) type_t config_provider_base_t::get_default_global_##name() const { return configuration->get()->global_##name(); }
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) type_t config_provider_base_t::get_default_local_##app##_##name() const { return configuration->get()->app_##app##_##name();  }
#define GAME_CONFIG_STD(type_t, name) type_t config_provider_base_t::get_default_cfg_##name(manifest_app_t::app_t app) const { return configuration->get()->cfg_##name(); }
#include "std/config_std.hpp"

	void config_provider_storage_t::rescan()
	{

#define GAME_CONFIG_GLOBAL_STD(type_t, name) vals.val_for_global_##name = platform::retrieve("global_" #name ##_u, get_default_global_##name());
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) vals.val_for_app_##app##_##name = platform::retrieve("local_" #app "_" #name ##_u, get_default_local_##app##_##name());
#define GAME_CONFIG_STD(type_t, name) for(auto i = 0; i < static_cast<std::underlying_type<manifest_app_t::app_t>::type>(manifest_app_t::app_t::count); i++) { vals.val_for_cfg_##name[i] = platform::retrieve(format_string("cfg_#1#_" #name ##_u, manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i))), get_default_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "std/config_std.hpp"
	}

	void config_provider_storage_t::notify_on_differences()
	{
#define GAME_CONFIG_GLOBAL_STD(type_t, name) if(vals.val_for_global_##name != old_vals.val_for_global_##name) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_global_##name = vals.val_for_global_##name; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::global_##name)); }
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) if(vals.val_for_app_##app##_##name != old_vals.val_for_app_##app##_##name) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_app_##app##_##name = vals.val_for_app_##app##_##name; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::app_##app##_##name)); }
#define GAME_CONFIG_STD(type_t, name) for(auto i = 0; i < static_cast<std::underlying_type<manifest_app_t::app_t>::type>(manifest_app_t::app_t::count); i++) {  if(vals.val_for_cfg_##name[i] != old_vals.val_for_cfg_##name[i]) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_cfg_##name[i] = vals.val_for_cfg_##name[i]; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::cfg_##name)); } }
#include "std/config_std.hpp"
	}
}



engine::config_real_t::config_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<holder_t<config_t> > config_provider, std::shared_ptr<logger_t> logger) :
	messenger(messenger),
	config_provider(std::move(config_provider)),
	logger(logger)
{
	auto task = logger->log_task_start(config, "Launching config subsystem..."_u);
	messenger->attach(msg_config_provider_updated_t::type, [this](msg_base_t* msg) { on_change_from_provider(msg); }, this);
	logger->log_task_done(task);

#define GAME_CONFIG_GLOBAL_STD(type_t, name) notify_on_initial_value(item_t::global_##name);
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) notify_on_initial_value(item_t::app_##app##_##name);
#define GAME_CONFIG_STD(type_t, name) notify_on_initial_value(item_t::cfg_##name);

#include "std/config_std.hpp"
}

engine::config_real_t::~config_real_t()
{
	auto task = logger->log_task_start(config, "Shutting down config subsystem..."_u);
	messenger->deatach_all(this);
	logger->log_task_done(task);
}

void engine::config_real_t::notify_on_initial_value(item_t item)
{
#define GAME_CONFIG_GLOBAL_STD(type_t, name) if(item == item_t::global_##name) logger->log_msg(config, "Config initial value for #1# is #2#"_u, type_to_text(item), get_global_##name());
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) if(item == item_t::app_##app##_##name) logger->log_msg(config, "Config initial value for #1# is #2#"_u, type_to_text(item), get_app_##app##_##name());
#define GAME_CONFIG_STD(type_t, name) if(item == item_t::cfg_##name) for(auto i = 0; i < static_cast<std::underlying_type<manifest_app_t::app_t>::type>(manifest_app_t::app_t::count); i++) {  logger->log_msg(config, "Config initial value for #1# (app: #2#) is #3#"_u, type_to_text(item), manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i)), get_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "std/config_std.hpp"

	messenger->post_message_sync(std::make_unique<msg_config_updated_t>(item, this));
}

void engine::config_real_t::notify_on_change(item_t item)
{
#define GAME_CONFIG_GLOBAL_STD(type_t, name) if(item == item_t::global_##name) logger->log_msg(config, "Config changed #1# -> #2#"_u, type_to_text(item), get_global_##name());
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) if(item == item_t::app_##app##_##name) logger->log_msg(config, "Config changed #1# -> #2#"_u, type_to_text(item), get_app_##app##_##name());
#define GAME_CONFIG_STD(type_t, name) if(item == item_t::cfg_##name) for(auto i = 0; i < static_cast<std::underlying_type<manifest_app_t::app_t>::type>(manifest_app_t::app_t::count); i++) {  logger->log_msg(config, "Config changed #1# (app: #2#) -> #3#"_u, type_to_text(item), manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i)), get_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "std/config_std.hpp"
	
	messenger->post_message_sync(std::make_unique<msg_config_updated_t>(item, this));
}

void engine::config_real_t::on_change_from_provider(msg_base_t * msg)
{
	if (msg->get_type() == msg_config_provider_updated_t::type)
	{
		msg_config_provider_updated_t* cfg_updated_msg = static_cast<msg_config_provider_updated_t*>(msg);
	
		notify_on_change(cfg_updated_msg->get_item());
	}
}
