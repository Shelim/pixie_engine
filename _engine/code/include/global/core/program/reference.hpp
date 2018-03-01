#ifndef ENGINE_GLOBAL_CORE_PROGRAM_REFERENCE_HPP
#define ENGINE_GLOBAL_CORE_PROGRAM_REFERENCE_HPP
#pragma once

namespace engine
{
    class program_t;

    class program_reference_t
    {

        public:

            program_reference_t(program_t * program) : program(program)
            {

            }

            program_t * get_program()
            {
                return program;
            }

        private:

            program_t * program;

    };

}

#endif