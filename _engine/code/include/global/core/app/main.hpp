#ifndef ENGINE_GLOBAL_CORE_APP_MAIN_HPP
#define ENGINE_GLOBAL_CORE_APP_MAIN_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{
    namespace client
    {
#define GAME_APP_IMPL(app) app_t::return_code_t main_##app(app_context_t* context);
#define GAME_APP_DEF(...) DEFINE_TYPE_PASS(GAME_APP_IMPL, __VA_ARGS__)
#include "def/app.def"
    }

}

#endif