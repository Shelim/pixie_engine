#ifndef ENGINE_GLOBAL_PROGRAM_HPP
#define ENGINE_GLOBAL_PROGRAM_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/extinguisher.hpp"

namespace engine
{

    namespace global
    {

        class program_t
        {

        public:

            void wait_for_completion()
            {

            }

            int32_t get_return_code()
            {
                return 0;
            }

            std::unique_ptr<extinguisher_t> get_extinguisher()
            {
                return std::make_unique<extinguisher_t>(this);
            }

        };

    }

}

#endif