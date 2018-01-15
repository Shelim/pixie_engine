#if PIXIE_WINDOWS

#if PIXIE_IS_ITERATIVE_BUILD
#include "autogen_windows_x86_iterative.cc"
#else

#if PIXIE_IS_DEBUG_BUILD

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x86_debug_portable.cc"
#else
#include "autogen_windows_x86_debug.cc"
#endif

#elif PIXIE_IS_FINAL_BUILD

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x86_final_portable_deploy.cc"
#else
#include "autogen_windows_x86_final_deploy.cc"
#endif

#else

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x86_hybrid_portable.cc"
#else
#if PIXIE_IS_DEPLOY_BUILD
#include "autogen_windows_x86_hybrid_deploy.cc"
#else
#include "autogen_windows_x86_hybrid.cc"
#endif
#endif

#endif
#endif

#else
#error This platform is not yet supported!
#endif