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
#include "local/core/manifest/app.hpp"
#include "utility/pattern/class_settings.hpp"
#include "global/component/config_common/enum.hpp"
#include "global/component/config_common/changed_provider.hpp"

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
                    virtual type get_local_##name(app_t::kind_t app) const = 0; \
                    virtual void set_local_##name(app_t::kind_t app, type val) = 0;
#include "def/config.def"

    protected:

        config_t()
        {

        }

    private:

    };

SETTINGS_TABLE_START(config_t)

#define ENGINE_CONFIG_GLOBAL(type, name) SETTINGS_TABLE_ENTRY(type, global_##name)
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) SETTINGS_TABLE_ENTRY(type, app_##app##_##name)
#define ENGINE_CONFIG_LOCAL(type, name) SETTINGS_TABLE_ENTRY(type, local_##name)
#include "def/config.def"

SETTINGS_TABLE_END()

}

#include "global/core/messenger/msg/config.hpp"
#include "global/component/config/dummy.hpp"
#include "global/component/config/real.hpp"

#endif