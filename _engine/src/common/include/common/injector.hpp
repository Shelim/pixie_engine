#ifndef ENGINE_COMMON_INJECTOR_HPP
#define ENGINE_COMMON_INJECTOR_HPP
#pragma once

#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 2

#include <boost/di.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>
#include "common/platform.hpp"
#include "common/config_container.hpp"
#include "common/std/config_std.hpp"

namespace di = boost::di;

auto make_di = [](const std::string& program_path)
{
	return di::make_injector(di::bind<std::string>().named(engine::arg0).to(program_path),
		di::bind<engine::config_io::provider_base_t>().to<engine::config_io::GAME_CONFIG_PROVIDER_STD>());
};

#endif