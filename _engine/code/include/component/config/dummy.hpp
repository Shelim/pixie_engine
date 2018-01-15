#ifndef ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#define ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#pragma once

#include "component/config.hpp"

namespace engine
{

	class config_dummy_t : public config_t
	{

	public:

		config_dummy_t(std::shared_ptr<manifest_app_t> manifest_app, std::unique_ptr<settings_t<config_t>> settings) : config_t(manifest_app), settings(std::move(settings))
		{
			
		}
		
#define ENGINE_CONFIG_GLOBAL(type, name) \
		type get_global_##name() const final { return settings->get()->global_##name(); } \
		void set_global_##name(type val) final { }

#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) \
		type get_app_##app##_##name() const final { return settings->get()->app_##app##_##name(); } \
		void set_app_##app##_##name(type val) final { }

#define ENGINE_CONFIG_LOCAL(type, name) \
		type get_local_##name(manifest_app_t::app_t app) const final { return settings->get()->local_##name(); } \
		void set_local_##name(manifest_app_t::app_t app, type val) final { }
#include "def/config.def"

	private:

		std::unique_ptr<settings_t<config_t>> settings;

	};
}

#endif