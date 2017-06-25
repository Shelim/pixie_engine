/// USING Google Test engine

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "common/utility/text/ustring.hpp"
#include "test/print_to.hpp"


TEST(ustring, formatting)
{
	EXPECT_EQ(engine::format_utf8(u8"#1#-#2#", _U("a"), _U("b")), _U("a-b"));
	EXPECT_EQ(engine::format_utf8(u8"#2#", _U("a"), _U("b")), _U("b"));
	EXPECT_EQ(engine::format_utf8(u8"#2#-#1#", _U("a"), _U("b")), _U("b-a"));
	EXPECT_EQ(engine::format_utf8(u8"#2#-#2#", _U("a"), _U("b"), _U("c")), _U("b-b"));
	EXPECT_EQ(engine::format_utf8(u8"#1#", _U("2"), _U("b"), _U("c")), _U("2"));
	EXPECT_EQ(engine::format_utf8(u8"#2#", _U("a"), _U("1"), _U("c")), _U("1"));
}

TEST(ustring, formatting_comments)
{
	EXPECT_EQ(engine::format_utf8(u8"#2#1#", _U("a"), _U("b")), _U("b1"));
	EXPECT_EQ(engine::format_utf8(u8"#2#1##", _U("a"), _U("b")), _U("b1#"));
	EXPECT_EQ(engine::format_utf8(u8"#1:This is sample comment#", _U("a"), _U("b")), _U("a"));
	EXPECT_EQ(engine::format_utf8(u8"#2:This is sample comment#1#test#", _U("a"), _U("b")), _U("b1"));
	EXPECT_EQ(engine::format_utf8(u8"#2:This is sample comment#1##test#", _U("a"), _U("b")), _U("b1#test"));
	EXPECT_EQ(engine::format_utf8(u8"#2#-#1:comment#-#1#-#2:comment!#-#1:2#", _U("a"), _U("b")), _U("b-a-a-b-a"));
	EXPECT_EQ(engine::format_utf8(u8"#2##1:comment##1##2:comment!##1:2#", _U("a"), _U("b")), _U("baaba"));
	EXPECT_EQ(engine::format_utf8(u8"#2##1:comment####1##2:comment!##1:2#", _U("a"), _U("b")), _U("ba#aba"));
}

TEST(ustring, formatting_escape)
{
	EXPECT_EQ(engine::format_utf8(u8"##", _U("a")), _U("#"));
	EXPECT_EQ(engine::format_utf8(u8"##1##", _U("a")), _U("#1#"));
	EXPECT_EQ(engine::format_utf8(u8"###2#", _U("a"), _U("b")), _U("#b"));
	EXPECT_EQ(engine::format_utf8(u8"#1##", _U("a"), _U("b")), _U("a"));
	EXPECT_EQ(engine::format_utf8(u8"#2:test###", _U("a"), _U("b")), _U("b#"));
}
