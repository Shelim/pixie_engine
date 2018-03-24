#include "global/core/app/resolver.hpp"

engine::app_t::kind_t engine::client::resolve_app(app_context_t * context)
{
    return engine::app_t::kind_t::game; // ToDo: This should be populated from context above!!!
}

engine::args_t engine::client::startup_args_to_run_new_instance_of(app_t::kind_t kind, args_t original_args)
{
    return original_args;  // ToDo: This should be populated from kind above!!!
}