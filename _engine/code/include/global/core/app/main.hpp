#ifndef ENGINE_GLOBAL_CORE_APP_MAIN_HPP
#define ENGINE_GLOBAL_CORE_APP_MAIN_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{
    namespace client
    {
#define GAME_APP_DEF(app) app_t::return_code_t main_##app(app_context_t* context);
#include "def/app.def"
    }

}

#endif