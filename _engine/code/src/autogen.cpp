#if PIXIE_WINDOWS

#if PIXIE_IS_ITERATIVE_BUILD
#include "autogen_windows_x64_iterative.cc"
#else

#if PIXIE_IS_TESTER_BUILD
#include "autogen_windows_x64_tester.cc"
#else

#if PIXIE_IS_DEBUG_BUILD

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x64_debug_portable.cc"
#else
#if PIXIE_IS_DEPLOY_BUILD
#include "autogen_windows_x64_debug_deploy.cc"
#else
#include "autogen_windows_x64_debug.cc"
#endif
#endif

#elif PIXIE_IS_FINAL_BUILD

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x64_final_portable_deploy.cc"
#else
#include "autogen_windows_x64_final_deploy.cc"
#endif

#else

#if PIXIE_IS_PORTABLE_BUILD
#include "autogen_windows_x64_hybrid_portable.cc"
#else
#if PIXIE_IS_DEPLOY_BUILD
#include "autogen_windows_x64_hybrid_deploy.cc"
#else
#include "autogen_windows_x64_hybrid.cc"
#endif
#endif

#endif
#endif
#endif

#else
#error This platform is not yet supported!
#endif