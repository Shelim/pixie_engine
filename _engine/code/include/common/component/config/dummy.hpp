#ifndef ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#define ENGINE_COMPONENT_CONFIG_DUMMY_HPP
#pragma once

#include "component/config.hpp"

namespace engine
{

	class config_dummy_t : public config_t
	{

	public:

		config_dummy_t(std::unique_ptr<settings_t<config_t>> configuration) : configuration(std::move(configuration))
		{
			
		}

#define GAME_CONFIG_STD(type, name) const type & get_##name() const final { return configuration->get()->name(); }; void set_##name(const type & val) final {  } 
#include "std/config_std.hpp"

	private:

		std::unique_ptr<settings_t<config_t>> configuration;

	};
}

#endif