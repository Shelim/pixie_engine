#include "global/core/manifest/version.hpp"
#include "global/core/manifest/manifest.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/text/ustring.hpp"
#include <array>

engine::version_t engine::manifest_version_t::version($VERSION_MAJOR$, $VERSION_MINOR$, $VERSION_REVISION$, $VERSION_BUILD$);

$MANIFEST_FIELDS$