#if PIXIE_IS_DEBUG_BUILD == $IS_DEBUG_BUILD$ && PIXIE_IS_PORTABLE_BUILD == $IS_PORTABLE_BUILD$

#include "common/manifest_app.hpp"

namespace engine
{
	int32_t manifest_app_t::version_major()
	{
		return $VERSION_MAJOR$;
	}
	int32_t manifest_app_t::version_minor()
	{
		return $VERSION_MINOR$;
	}
	int32_t manifest_app_t::version_revision()
	{
		return $VERSION_REVISION$;
	}
	int32_t manifest_app_t::version_build()
	{
		return $VERSION_BUILD$;
	}
	const char * manifest_app_t::name()
	{
		return "$NAME$";
	}
	const char * manifest_app_t::unix_name()
	{
		return "$UNIX_NAME$";
	}
}

#endif