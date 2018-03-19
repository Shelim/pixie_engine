#ifndef ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#define ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#pragma once

#include "global/component/config.hpp"

namespace engine
{

	class config_dummy_t : public config_t
	{

	public:

		config_dummy_t(std::unique_ptr<settings_t<config_t>> settings) : settings(std::move(settings))
		{
			
		}
		
#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) \
		type get_global_##name() const final { return settings->get()->global_##name(); } \
		void set_global_##name(type val) final { }
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)

#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) \
		type get_app_##app##_##name() const final { return settings->get()->app_##app##_##name(); } \
		void set_app_##app##_##name(type val) final { }
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)

#define ENGINE_CONFIG_LOCAL_IMPL(name, type) \
		type get_local_##name(app_t::kind_t app) const final { return settings->get()->local_##name(); } \
		void set_local_##name(app_t::kind_t app, type val) final { }
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)

#include "def/config.def"


	private:

		std::unique_ptr<settings_t<config_t>> settings;

	};
}

#endif