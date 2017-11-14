#include "component/config_storage.hpp"

engine::config_storage_t::~config_storage_t()
{
			
}

void engine::config_storage_t::store(const ustring_t & key, bool val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, uint8_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, uint16_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, uint32_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, uint64_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, int8_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, int16_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, int32_t val)
{
	store(key, to_string(val));
}
void engine::config_storage_t::store(const ustring_t & key, int64_t val)
{
	store(key, to_string(val));
}

bool engine::config_storage_t::retrieve(const ustring_t & key, bool def_val)
{
	return from_string<bool>(retrieve(key, to_string(def_val)));
}
uint8_t engine::config_storage_t::retrieve(const ustring_t & key, uint8_t def_val)
{
	return from_string<uint8_t>(retrieve(key, to_string(def_val)));
}
uint16_t engine::config_storage_t::retrieve(const ustring_t & key, uint16_t def_val)
{
	return from_string<uint16_t>(retrieve(key, to_string(def_val)));
}
uint32_t engine::config_storage_t::retrieve(const ustring_t & key, uint32_t def_val)
{
	return from_string<uint32_t>(retrieve(key, to_string(def_val)));
}
uint64_t engine::config_storage_t::retrieve(const ustring_t & key, uint64_t def_val)
{
	return from_string<uint64_t>(retrieve(key, to_string(def_val)));
}
int8_t engine::config_storage_t::retrieve(const ustring_t & key, int8_t def_val)
{
	return from_string<int8_t>(retrieve(key, to_string(def_val)));
}
int16_t engine::config_storage_t::retrieve(const ustring_t & key, int16_t def_val)
{
	return from_string<int16_t>(retrieve(key, to_string(def_val)));
}
int32_t engine::config_storage_t::retrieve(const ustring_t & key, int32_t def_val)
{
	return from_string<int32_t>(retrieve(key, to_string(def_val)));
}
int64_t engine::config_storage_t::retrieve(const ustring_t & key, int64_t def_val)
{
	return from_string<int64_t>(retrieve(key, to_string(def_val)));
}

engine::config_storage_dummy_t::config_storage_dummy_t()
{
	
}

void engine::config_storage_dummy_t::store(const ustring_t & key, const ustring_t & val)
{
	values[key] = val;
}

engine::ustring_t engine::config_storage_dummy_t::retrieve(const ustring_t & key, const ustring_t & def_val)
{
	auto iter = values.find(key);
	if (iter == values.end())
	{
		return def_val;
	}
	return iter->second;
}

engine::config_storage_real_t::config_storage_real_t(std::unique_ptr<holder_t<config_storage_t> > config_storage_provider) : config_storage_provider(std::move(config_storage_provider))
{

}

void engine::config_storage_real_t::store(const ustring_t & key, bool val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, uint8_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, uint16_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, uint32_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, uint64_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, int8_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, int16_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, int32_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, int64_t val)
{
	config_storage_provider->get_provider()->store(key, val);
}
void engine::config_storage_real_t::store(const ustring_t & key, const ustring_t & val)
{
	config_storage_provider->get_provider()->store(key, val);
}

bool engine::config_storage_real_t::retrieve(const ustring_t & key, bool def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
uint8_t engine::config_storage_real_t::retrieve(const ustring_t & key, uint8_t def_val)
{ 
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
uint16_t engine::config_storage_real_t::retrieve(const ustring_t & key, uint16_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
uint32_t engine::config_storage_real_t::retrieve(const ustring_t & key, uint32_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
uint64_t engine::config_storage_real_t::retrieve(const ustring_t & key, uint64_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
int8_t engine::config_storage_real_t::retrieve(const ustring_t & key, int8_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
int16_t engine::config_storage_real_t::retrieve(const ustring_t & key, int16_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
int32_t engine::config_storage_real_t::retrieve(const ustring_t & key, int32_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
int64_t engine::config_storage_real_t::retrieve(const ustring_t & key, int64_t def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}
engine::ustring_t engine::config_storage_real_t::retrieve(const ustring_t & key, const ustring_t & def_val)
{
	return config_storage_provider->get_provider()->retrieve(key, def_val);
}


/*

#include "component/config.hpp"
#include "component/config/msg_config_updated.hpp"
#include "utility/platform/config_storage.hpp"
#include "utility/manifest/app.hpp"
#include "utility/pattern/enum.hpp"


namespace engine
{
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) type_t config_provider_base_t::provider_default_t::get_default_global_##name() const { return configuration->get()->global_##name(); }
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) type_t config_provider_base_t::provider_default_t::get_default_local_##app##_##name() const { return configuration->get()->app_##app##_##name();  }
#define ENGINE_CONFIG_DEF(type_t, name) type_t config_provider_base_t::provider_default_t::get_default_cfg_##name(manifest_app_t::app_t app) const { return configuration->get()->cfg_##name(); }
#include "def/config.def"

	void config_provider_storage_actual_t::rescan()
	{

#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) vals.val_for_global_##name = config_storage->retrieve("global_" #name ##_u, defaults->get_default_global_##name());
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) vals.val_for_app_##app##_##name = config_storage->retrieve("local_" #app "_" #name ##_u, defaults->get_default_local_##app##_##name());
#define ENGINE_CONFIG_DEF(type_t, name) for(auto i = 0; i < value_of(manifest_app_t::app_t::count); i++) { vals.val_for_cfg_##name[i] = config_storage->retrieve(format_string("cfg_#1#_" #name ##_u, manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i))), defaults->get_default_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "def/config.def"
	}

	void config_provider_storage_actual_t::notify_on_differences()
	{
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) if(vals.val_for_global_##name != old_vals.val_for_global_##name) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_global_##name = vals.val_for_global_##name; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::global_##name)); }
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) if(vals.val_for_app_##app##_##name != old_vals.val_for_app_##app##_##name) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_app_##app##_##name = vals.val_for_app_##app##_##name; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::app_##app##_##name)); }
#define ENGINE_CONFIG_DEF(type_t, name) for(auto i = 0; i < value_of(manifest_app_t::app_t::count); i++) {  if(vals.val_for_cfg_##name[i] != old_vals.val_for_cfg_##name[i]) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); rdy_vals.val_for_cfg_##name[i] = vals.val_for_cfg_##name[i]; messenger->post_message(std::make_unique<msg_config_provider_updated_t>(config_t::item_t::cfg_##name)); } }
#include "def/config.def"
	}
}



engine::config_real_t::config_real_t(std::shared_ptr<manifest_app_t> manifest_app, std::shared_ptr<messenger_t> messenger, std::unique_ptr<holder_t<config_t> > config_provider, std::shared_ptr<logger_t> logger) :
	config_t(manifest_app),
	messenger(messenger),
	config_provider(std::move(config_provider)),
	logger(logger),
	callbacks_container(messenger, this)
{
	auto task = logger->log_task_start(config, "Launching config subsystem"_u);
	callbacks_container.attach(msg_config_provider_updated_t::type, [this](msg_base_t* msg) { on_change_from_provider(msg); });
	logger->log_task_done(task);

#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) notify_on_initial_value(item_t::global_##name);
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) notify_on_initial_value(item_t::app_##app##_##name);
#define ENGINE_CONFIG_DEF(type_t, name) notify_on_initial_value(item_t::cfg_##name);

#include "def/config.def"
}

engine::config_real_t::~config_real_t()
{
	auto task = logger->log_task_start(config, "Shutting down config subsystem"_u);
	logger->log_task_done(task);
}

void engine::config_real_t::notify_on_initial_value(item_t item)
{
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) if(item == item_t::global_##name) logger->log_msg(config, "Config initial value for #1# is #2#"_u, type_to_text(item), get_global_##name());
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) if(item == item_t::app_##app##_##name) logger->log_msg(config, "Config initial value for #1# is #2#"_u, type_to_text(item), get_app_##app##_##name());
#define ENGINE_CONFIG_DEF(type_t, name) if(item == item_t::cfg_##name) for(auto i = 0; i < value_of(manifest_app_t::app_t::count); i++) {  logger->log_msg(config, "Config initial value for #1# (app: #2#) is #3#"_u, type_to_text(item), manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i)), get_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "def/config.def"

	messenger->post_message_sync(std::make_unique<msg_config_updated_t>(item, this));
}

void engine::config_real_t::notify_on_change(item_t item)
{
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) if(item == item_t::global_##name) logger->log_msg(config, "Config changed #1# -> #2#"_u, type_to_text(item), get_global_##name());
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) if(item == item_t::app_##app##_##name) logger->log_msg(config, "Config changed #1# -> #2#"_u, type_to_text(item), get_app_##app##_##name());
#define ENGINE_CONFIG_DEF(type_t, name) if(item == item_t::cfg_##name) for(auto i = 0; i < value_of(manifest_app_t::app_t::count); i++) {  logger->log_msg(config, "Config changed #1# (app: #2#) -> #3#"_u, type_to_text(item), manifest_app_t::get_app_name(static_cast<manifest_app_t::app_t>(i)), get_cfg_##name(static_cast<manifest_app_t::app_t>(i))); }
#include "def/config.def"
	
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

*/