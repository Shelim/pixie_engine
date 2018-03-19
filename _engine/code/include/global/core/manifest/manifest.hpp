#ifndef ENGINE_COMMON_UTILITY_MANIFEST_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_HPP

#include "utility/text/ustring.hpp"
#include "utility/pattern/enum.hpp"

#pragma once

namespace engine
{

#define ENGINE_MANIFEST_CLASS_NAME2(name) class manifest_##name##_t
#define ENGINE_MANIFEST_CLASS_NAME(name) ENGINE_MANIFEST_CLASS_NAME2(name)
#define ENGINE_MANIFEST_IMPL3(name) ustring_t get_##name() const;
#define ENGINE_MANIFEST_IMPL2(...) DEFINE_TYPE_PASS3(ENGINE_MANIFEST_IMPL3, __VA_ARGS__)
#define ENGINE_MANIFEST_IMPL(name, ...) ENGINE_MANIFEST_CLASS_NAME(EXPAND name) { public: DEFINE_TYPE_PASS2(ENGINE_MANIFEST_IMPL2, __VA_ARGS__) };
#define ENGINE_MANIFEST_DEF(...) DEFINE_TYPE_PASS(ENGINE_MANIFEST_IMPL, __VA_ARGS__)
#include "def/manifest.def"
#undef ENGINE_MANIFEST_IMPL2
#undef ENGINE_MANIFEST_IMPL3
#undef ENGINE_MANIFEST_CLASS_NAME
#undef ENGINE_MANIFEST_CLASS_NAME2

}

#endif