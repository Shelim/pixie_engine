#include "component/config.hpp"
#include "utility/messenger/msg_config_updated.hpp"
#include "platform/config_storage.hpp"

namespace engine
{
#define GAME_CONFIG_STD(type, name) const type & config_provider_base_t::get_default_##name() const { return configuration->get()->name(); };
#include "std/config_std.hpp"

	void config_provider_storage_t::rescan()
	{
#define GAME_CONFIG_STD(type, name) vals.val_for_##name = platform::retrieve(ustring_t::from_utf8(#name), get_default_##name());
#include "std/config_std.hpp"
	}

	void config_provider_storage_t::notify_on_differences()
	{
#define GAME_CONFIG_STD(type, name) if(vals.val_for_##name != old_vals.val_for_##name) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_##name = vals.val_for_##name; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::name)); }
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

#define GAME_CONFIG_STD(type, name) notify_on_initial_value(item_t::name);
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
#define GAME_CONFIG_STD(type, name) if(item == item_t::name) logger->log_msg(config, "Config initial value for #1# is #2#"_u, type_to_text(item), get_##name());
#include "std/config_std.hpp"

	messenger->post_message_sync(std::make_unique<msg_config_updated_t>(item, this));
}

void engine::config_real_t::notify_on_change(item_t item)
{
#define GAME_CONFIG_STD(type, name) if(item == item_t::name) logger->log_msg(config, "Config changed #1# -> #2#"_u, type_to_text(item), get_##name());
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