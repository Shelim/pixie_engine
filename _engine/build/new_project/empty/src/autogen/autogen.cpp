#if PIXIE_WINDOWS

#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_dll

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/dll/autogen_debug_portable.hpp"
#else
#include "windows_x86/dll/autogen_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/dll/autogen_release_portable.hpp"
#else
#include "windows_x86/dll/autogen_release.hpp"
#endif
#endif

#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_editor

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/editor/autogen_debug_portable.hpp"
#else
#include "windows_x86/editor/autogen_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/editor/autogen_release_portable.hpp"
#else
#include "windows_x86/editor/autogen_release.hpp"
#endif
#endif

#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_game

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/game/autogen_debug_portable.hpp"
#else
#include "windows_x86/game/autogen_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/game/autogen_release_portable.hpp"
#else
#include "windows_x86/game/autogen_release.hpp"
#endif
#endif

#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_launcher

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/launcher/autogen_debug_portable.hpp"
#else
#include "windows_x86/launcher/autogen_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/launcher/autogen_release_portable.hpp"
#else
#include "windows_x86/launcher/autogen_release.hpp"
#endif
#endif

#elif PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_test

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/test/autogen_debug_portable.hpp"
#else
#include "windows_x86/test/autogen_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "windows_x86/test/autogen_release_portable.hpp"
#else
#include "windows_x86/test/autogen_release.hpp"
#endif
#endif

#else

#error "Autogen requires known output type - did you forgot to define PIXIE_OUTPUT_TYPE?"

#endif

#else

#error "Autogen requires known platform - did you forgot to define PIXIE_PLATFORM_FULL?"

#endif