#ifndef ENGINE_COMPONENT_PROGRAM_INSTANCER_GENERIC_HPP
#define ENGINE_COMPONENT_PROGRAM_INSTANCER_GENERIC_HPP
#pragma once

#include "global/component/program_instancer/real.hpp"

namespace engine
{
	class program_instancer_provider_generic_t : public program_instancer_provider_base_t
	{

        public:
        
            bool run_new_program(args_t args) final
            {
                ustring_t cmd = args.get_args();
                system(cmd.get_cstring());
                return true;
            }

    };
}

#endif