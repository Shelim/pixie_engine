#ifndef ENGINE_GLOBAL_COMPONENT_DEBUG_SYMBOLS_HPP
#define ENGINE_GLOBAL_COMPONENT_DEBUG_SYMBOLS_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class debug_symbols_t
    {

    public:

        virtual ~debug_symbols_t()
        {

        }

    };

}

#include "global/component/debug_symbols/dummy.hpp"
#include "global/component/debug_symbols/real.hpp"

#endif