#ifndef ENGINE_CORE_CONSOLE_MSG_META_HPP
#define ENGINE_CORE_CONSOLE_MSG_META_HPP
#pragma once

#include <chrono>
#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/debug/callstack.hpp"
#include "utility/platform/debug.hpp"
#include "global/core/messenger/msg/console.hpp"

namespace engine
{
    namespace console
    {

        class meta_item_t final : public messenger::msg_console_t::actual_t
        {
        public:

#include "def/enum/console_meta.def"

            meta_item_t(meta_type_t meta_type, type_t type);

            meta_type_t get_meta_type() const;
            type_t get_meta_type_type() const;
            
        private:

            meta_type_t meta_type;
            type_t type;

        };

    }

}


#define ENGINE_ENUM_HEADER_TO_USE "def/enum/console_meta.def"
#include "global/core/utility/enum_to_string.hpp"

#endif