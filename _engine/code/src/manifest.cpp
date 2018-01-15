#include "core/manifest/app.hpp"
#include "core/manifest/manifest.hpp"
#include "core/manifest/version.hpp"

engine::manifest_app_t::app_t engine::manifest_app_t::get_local_app()
{
    return local_app;
}

engine::manifest_app_t::manifest_app_t(app_t local_app) : local_app(local_app)
{

}