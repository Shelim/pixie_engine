#if PIXIE_OUTPUT_TYPE == PIXIE_OUTPUT_TYPE_$OUTPUT_TYPE$ && PIXIE_IS_DEBUG_BUILD == $IS_DEBUG_BUILD$ && PIXIE_IS_PORTABLE_BUILD == $IS_PORTABLE_BUILD$

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
	const char * manifest_app_t::game_name()
	{
		return "$GAME_NAME$";
	}
	const char * manifest_app_t::unix_name()
	{
		return "$UNIX_NAME$";
	}
	const char * manifest_app_t::website_url()
	{
		return "$WEBSITE_URL$";
	}
	const char * manifest_app_t::support_page_url()
	{
		return "$SUPPORT_PAGE_URL$";
	}
	const char * manifest_app_t::updater_manifest_url()
	{
		return "$UPDATER_MANIFEST_URL$";
	}
	const char * manifest_app_t::game_command()
	{
		return "$GAME_COMMAND$";
	}
	const char * manifest_app_t::editor_command()
	{
		return "$EDITOR_COMMAND$";
	}
	const char * manifest_app_t::windows_game_app_dir()
	{
		return "$WINDOWS_GAME_APP_DIR$";
	}
}

#endif