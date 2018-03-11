#include "global/core/app/bootstrapper.hpp"

engine::app_t::return_code_t engine::client::main_game(app_context_t* context)
{
    BOOTSTRAP_APP(game, context)


    return engine::app_t::return_code_t::success;
}