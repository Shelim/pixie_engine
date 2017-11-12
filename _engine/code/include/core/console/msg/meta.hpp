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
    #define ENGINE_CONSOLE_META_TYPE_STD(meta) meta,
    #include "std/console_std.hpp"
                count
            };

            meta_item_t(meta_type_t meta_type) :
                actual_t(type_t::meta, source_t::core, kind_t::meta), meta_type(meta_type)
            {

            }

            meta_type_t get_meta_type()
            {
                return meta_type;
            }

        private:

            meta_type_t meta_type;

        };

    }
}

#endif