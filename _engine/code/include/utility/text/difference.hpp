#ifndef ENGINE_COMMON_UTILITY_TEXT_SIMILARITY_HPP
#define ENGINE_COMMON_UTILITY_TEXT_SIMILARITY_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include <levenshtein.h>

namespace engine
{
    size_t string_difference(const ustring_t & str1, const ustring_t & str2)
    {
        return LevenshteinDistance(str1.begin(), str1.end(), str2.begin(), str2.end());
    }

}

#endif