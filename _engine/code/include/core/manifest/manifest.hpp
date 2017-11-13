#ifndef ENGINE_COMMON_UTILITY_MANIFEST_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

#define ENGINE_MANIFEST_START_STD(manifest_type) class manifest_##manifest_type##_t { public:
#define ENGINE_MANIFEST_STD(manifest_type, name) ustring_t get_##name() const;
#define ENGINE_MANIFEST_END_STD() };
#include "def/manifest.def"

}

#endif