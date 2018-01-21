#ifndef ENGINE_GLOBAL_CORE_PROGRAM_REAL_HPP
#define ENGINE_GLOBAL_CORE_PROGRAM_REAL_HPP
#pragma once

#include "global/core/program.hpp"

namespace engine
{

    namespace global
    {
        class program_real_t : public program_t
        {

        public:

            program_real_t()
            {

            }

            void wait_for_completion() final
            {

            }

            int32_t get_return_code() const final
            {
                return 0;
            }

        private:

        };
    }

}

#endif