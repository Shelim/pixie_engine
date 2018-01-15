#ifndef ENGINE_COMPONENT_CONFIG_HPP
#define ENGINE_COMPONENT_CONFIG_HPP
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <array>
#include "utility/text/ustring.hpp"
#include "core/manifest/app.hpp"
#include "utility/pattern/class_settings.hpp"
#include "component/config_common/enum.hpp"
#include "component/config_common/changed_provider.hpp"

namespace engine
{
    
    class config_t
    {

    public:

		virtual ~config_t()
        {

        }
        
#define ENGINE_CONFIG_GLOBAL(type, name) \
                    virtual type get_global_##name() const = 0; \
                    virtual void set_global_##name(type val) = 0;

#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) \
                     virtual type get_app_##app##_##name() const = 0; \
                     virtual void set_app_##app##_##name(type val) = 0;

#define ENGINE_CONFIG_LOCAL(type, name) \
                    virtual type get_local_##name(manifest_app_t::app_t app) const = 0; \
                    virtual void set_local_##name(manifest_app_t::app_t app, type val) = 0; \
                    type get_local_##name() const { return get_local_##name(manifest_app->get_local_app()); } \
                    void set_local_##name(type val) { set_local_##name(manifest_app->get_local_app(), val); }
#include "def/config.def"

    protected:

        config_t(std::shared_ptr<manifest_app_t> manifest_app) : manifest_app(manifest_app)
        {

        }

        std::shared_ptr<manifest_app_t> get_manifest_app() const
        {
            return manifest_app;
        }

    private:
    
        std::shared_ptr<manifest_app_t> manifest_app;

    };

SETTINGS_TABLE_START(config_t)

#define ENGINE_CONFIG_GLOBAL(type, name) SETTINGS_TABLE_ENTRY(type, global_##name)
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) SETTINGS_TABLE_ENTRY(type, app_##app##_##name)
#define ENGINE_CONFIG_LOCAL(type, name) SETTINGS_TABLE_ENTRY(type, local_##name)
#include "def/config.def"

SETTINGS_TABLE_END()

}

#include "core/messenger/msg/config.hpp"
#include "component/config/dummy.hpp"
#include "component/config/real.hpp"

#endif