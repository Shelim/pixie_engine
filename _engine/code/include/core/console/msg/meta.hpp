#ifndef ENGINE_CORE_CONSOLE_MSG_META_HPP
#define ENGINE_CORE_CONSOLE_MSG_META_HPP
#pragma once

#include <chrono>
#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/debug/callstack.hpp"
#include "utility/platform/debug.hpp"
#include "core/messenger/msg/console.hpp"

namespace engine
{
    namespace console
    {

        class meta_item_t final : public messenger::msg_console_t::actual_t
        {
        public:

            enum meta_type_t
            {
    #define ENGINE_CONSOLE_META_TYPE_DEF(meta) meta,
    #include "def/console.def"
                count
            };

            meta_item_t(meta_type_t meta_type, type_t type) :
                actual_t(type_t::meta, source_t::core, kind_t::meta), meta_type(meta_type), type(type)
            {

            }

            meta_type_t get_meta_type() const
            {
                return meta_type;
            }

            type_t get_meta_type_type() const
            {
                return type;
            }

        private:

            meta_type_t meta_type;
            type_t type;

        };

    }

#define STRINGIFY_ENUM_TYPE console::meta_item_t::meta_type_t
#define ENGINE_CONSOLE_META_TYPE_DEF STRINGIFY_DEF_NAME
#define STRINGIFY_DEF_INCLUDE "def/console.def"
#include "core/utility/stringify_def.hpp"

}

#endif