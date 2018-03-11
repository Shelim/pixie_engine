#include "global/core/app/bootstrapper.hpp"

engine::app_t::return_code_t engine::client::main_editor(app_context_t* context)
{
    BOOTSTRAP_APP(editor, context)

    
    return engine::app_t::return_code_t::success;
}