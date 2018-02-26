#include <gtest/gtest.h>
#include "utility/text/ustring.hpp"

TEST(UtilityTextUstring, EqualityUtf8)
{
    EXPECT_EQ("sample text"_u, engine::ustring_t::from_utf8("sample text"));
}
