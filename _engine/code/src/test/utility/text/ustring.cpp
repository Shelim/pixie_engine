#include <gtest/gtest.h>
#include "utility/text/ustring.hpp"

TEST(UtilityTextUstringSymbol, ToUpper)
{
    EXPECT_EQ('A', engine::to_upper('a'));
    EXPECT_EQ('A', engine::to_upper('A'));
    EXPECT_EQ('=', engine::to_upper('='));
    EXPECT_EQ(';', engine::to_upper(';'));
    EXPECT_EQ(u'\u20AC', engine::to_upper(u'\u20AC'));
    EXPECT_EQ('\0', engine::to_upper('\0'));
}
TEST(UtilityTextUstringSymbol, ToLower)
{
    EXPECT_EQ('a', engine::to_lower('a'));
    EXPECT_EQ('a', engine::to_lower('A'));
    EXPECT_EQ('=', engine::to_lower('='));
    EXPECT_EQ(';', engine::to_lower(';'));
    EXPECT_EQ(u'\u20AC', engine::to_lower(u'\u20AC'));
    EXPECT_EQ(u'\u20AC', engine::to_lower(u'\u20AC'));
}
TEST(UtilityTextUstringSymbol, IsWhitespaceAscii)
{
    EXPECT_TRUE(engine::is_whitespace_ascii(' '));
    EXPECT_TRUE(engine::is_whitespace_ascii('\t'));
    EXPECT_TRUE(engine::is_whitespace_ascii('\n'));
    EXPECT_TRUE(engine::is_whitespace_ascii('\r'));
    EXPECT_FALSE(engine::is_whitespace_ascii('a'));
    EXPECT_FALSE(engine::is_whitespace_ascii(';'));
    EXPECT_FALSE(engine::is_whitespace_ascii('='));
    EXPECT_FALSE(engine::is_whitespace_ascii(u'\u20AC'));
}
TEST(UtilityTextUstringSymbol, IsPathSeparatorAscii)
{
    EXPECT_TRUE(engine::is_path_separator_ascii('/'));
    EXPECT_FALSE(engine::is_path_separator_ascii(' '));
    EXPECT_FALSE(engine::is_path_separator_ascii('a'));
    EXPECT_FALSE(engine::is_path_separator_ascii(';'));
    EXPECT_FALSE(engine::is_path_separator_ascii('='));
    EXPECT_FALSE(engine::is_path_separator_ascii(u'\u20AC'));
}
TEST(UtilityTextUstringSymbol, IsNumeric)
{
    EXPECT_TRUE(engine::is_numeric('0'));
    EXPECT_TRUE(engine::is_numeric('1'));
    EXPECT_TRUE(engine::is_numeric('2'));
    EXPECT_TRUE(engine::is_numeric('3'));
    EXPECT_TRUE(engine::is_numeric('4'));
    EXPECT_TRUE(engine::is_numeric('5'));
    EXPECT_TRUE(engine::is_numeric('6'));
    EXPECT_TRUE(engine::is_numeric('7'));
    EXPECT_TRUE(engine::is_numeric('8'));
    EXPECT_TRUE(engine::is_numeric('9'));
    EXPECT_FALSE(engine::is_numeric('a'));
    EXPECT_FALSE(engine::is_numeric('b'));
    EXPECT_FALSE(engine::is_numeric(';'));
    EXPECT_FALSE(engine::is_numeric('='));
    EXPECT_FALSE(engine::is_numeric('\0'));
    EXPECT_FALSE(engine::is_numeric(u'\u20AC'));
}
TEST(UtilityTextUstringSymbol, IsCharacterAscii)
{
    EXPECT_TRUE(engine::is_character_ascii('a'));
    EXPECT_TRUE(engine::is_character_ascii('d'));
    EXPECT_TRUE(engine::is_character_ascii('z'));
    EXPECT_TRUE(engine::is_character_ascii('A'));
    EXPECT_TRUE(engine::is_character_ascii('D'));
    EXPECT_TRUE(engine::is_character_ascii('Z'));
    EXPECT_FALSE(engine::is_character_ascii('3'));
    EXPECT_FALSE(engine::is_character_ascii('5'));
    EXPECT_FALSE(engine::is_character_ascii('@'));
    EXPECT_FALSE(engine::is_character_ascii('#'));
    EXPECT_FALSE(engine::is_character_ascii('\n'));
    EXPECT_FALSE(engine::is_character_ascii('\0'));
    EXPECT_FALSE(engine::is_character_ascii(u'\u20AC'));
    EXPECT_FALSE(engine::is_character_ascii(u'\u017C'));

}