#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_DATA_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_DATA_HPP
#pragma once

#include "component/config/real.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class config_provider_data_t : public config_provider_base_t
	{

	public:

		config_provider_data_t(std::unique_ptr<settings_t<config_t>> configuration) : config_provider_base_t(std::move(configuration))
		{

		}

		~config_provider_data_t()
		{

		}

#define GAME_CONFIG_STD(type, name) const type & get_##name() const final { return get_default_##name(); };
#include "std/config_std.hpp"
#define GAME_CONFIG_STD(type, name) bool set_##name(const type & val) final { return true; };
#include "std/config_std.hpp"

	private:


	};

}

#endif