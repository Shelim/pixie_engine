#ifndef ENGINE_GLOBAL_EXTINGUISHER_HPP
#define ENGINE_GLOBAL_EXTINGUISHER_HPP
#pragma once

namespace engine
{

    namespace global
    {
        class program_t;

        class extinguisher_t
        {

        public:

            extinguisher_t(program_t * program) : program(program)
            {

            }

            void extinguish()
            {
                
            }

        private:

            program_t * program;

        };

    }

}

#endif