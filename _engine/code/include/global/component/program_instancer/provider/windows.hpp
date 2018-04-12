#ifndef ENGINE_COMPONENT_PROGRAM_INSTANCER_WINDOWS_HPP
#define ENGINE_COMPONENT_PROGRAM_INSTANCER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/program_instancer/real.hpp"
#include <windows.h>

namespace engine
{
	class program_instancer_provider_windows_t : public program_instancer_provider_base_t
	{

        public:
        
            bool run_new_program(args_t args) final
            {
                return true;
                /*
                STARTUPINFO si;     
                PROCESS_INFORMATION pi;

                // set the size of the structures
                ZeroMemory( &si, sizeof(si) );
                si.cb = sizeof(si);
                ZeroMemory( &pi, sizeof(pi) );

                // start the program up
                CreateProcess( lpApplicationName,   // the path
                argv[1],        // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory 
                &si,            // Pointer to STARTUPINFO structure
                &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
                );
                // Close process and thread handles. 
                CloseHandle( pi.hProcess );
                CloseHandle( pi.hThread );
                */
            }
    };
}

#endif

#endif