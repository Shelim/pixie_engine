#include "local/core/manifest/app.hpp"
#include "global/core/manifest/manifest.hpp"
#include "global/core/manifest/version.hpp"


engine::app_t::kind_t engine::manifest_app_t::get_local_app() const
{
    return local_app;
}

engine::app_t::instance_id_t engine::manifest_app_t::get_local_instance_id() const
{
    return local_instance_id;
}

engine::manifest_app_t::manifest_app_t(app_t::kind_t local_app, engine::app_t::instance_id_t local_instance_id) : local_app(local_app), local_instance_id(local_instance_id)
{

}