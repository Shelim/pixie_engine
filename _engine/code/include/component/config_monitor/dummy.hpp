#ifndef ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#define ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#pragma once

#include "component/config.hpp"
#include "utility/manifest/app.hpp"
#include "utility/pattern/enum.hpp"

namespace engine
{

	class config_dummy_t : public config_t
	{

	public:

		config_dummy_t(std::shared_ptr<manifest_app_t> manifest_app, std::unique_ptr<settings_t<config_t>> configuration) : config_t(manifest_app), configuration(std::move(configuration))
		{
			
		}


#define ENGINE_CONFIG_GLOBAL_DEF(type, name) type get_global_##name() const final { return configuration->get()->global_##name(); }  virtual void set_global_##name(type val) final { }
#define ENGINE_CONFIG_LOCAL_DEF(type, app, name) type get_app_##app##_##name() const final { return configuration->get()->app_##app##_##name(); }  virtual void set_app_##app##_##name(type val) final { }
#define ENGINE_CONFIG_DEF(type, name) type get_cfg_##name(manifest_app_t::app_t app) const final { return configuration->get()->cfg_##name(); } virtual void set_cfg_##name(manifest_app_t::app_t app, type val) final { }
#include "def/config.def"

	private:

		std::unique_ptr<settings_t<config_t>> configuration;

	};
}

#endif