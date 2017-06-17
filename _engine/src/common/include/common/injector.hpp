#ifndef ENGINE_COMMON_INJECTOR_HPP
#define ENGINE_COMMON_INJECTOR_HPP
#pragma once

#define BOOST_DI_CFG_DIAGNOSTICS_LEVEL 2
// #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 50

#include <boost/di.hpp>
#include <functional>

namespace di = boost::di;

namespace engine
{
	extern std::function<void()> arg0;
}

#endif