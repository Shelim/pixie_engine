#if PIXIE_WINDOWS

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "_autogen/windows_x86/$EMPTY_UNIX_NAME$/app_list_std_debug_portable.hpp"
#else
#include "_autogen/windows_x86/$EMPTY_UNIX_NAME$/app_list_std_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "_autogen/windows_x86/$EMPTY_UNIX_NAME$/app_list_std_release_portable.hpp"
#else
#include "_autogen/windows_x86/$EMPTY_UNIX_NAME$/app_list_std_release.hpp"
#endif
#endif

#else

#error "Autogen requires known platform - did you forgot to define PIXIE_PLATFORM_FULL?"

#endif