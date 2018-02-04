#include "global/core/manifest/version.hpp"
#include "global/core/manifest/manifest.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/text/ustring.hpp"
#include <array>

uint32_t engine::manifest_version_t::get_major() const
{
	return $VERSION_MAJOR$;
}
uint32_t engine::manifest_version_t::get_minor() const
{
	return $VERSION_MINOR$;
}
uint32_t engine::manifest_version_t::get_revision() const
{
	return $VERSION_REVISION$;
}
uint32_t engine::manifest_version_t::get_build() const
{
	return $VERSION_BUILD$;
}

$MANIFEST_FIELDS$